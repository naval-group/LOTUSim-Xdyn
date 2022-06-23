#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import math
import os
import sys

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from numpy import exp, log


def which(program):
    """retur full path to program if found, else return None"""

    def is_exe(fpath):
        return os.path.isfile(fpath) and os.access(fpath, os.X_OK)

    fpath, _ = os.path.split(program)
    if fpath:
        if is_exe(program):
            return program
    else:
        for path in os.environ["PATH"].split(os.pathsep):
            exe_file = os.path.join(path, program)
            if is_exe(exe_file):
                return exe_file
    return None


matplotlib.use("AGG")
matplotlib.rc("grid", linewidth=0.5, linestyle="--", color="#bdbdbd")
matplotlib.rc("xtick", labelsize=14)
matplotlib.rc("ytick", labelsize=14)
latex_program = "latex" + ".exe" if sys.platform == "win32" else ""
matplotlib.rc("text", usetex=True if which(latex_program) else False)
matplotlib.rc("font", **{"family": "serif", "serif": ["Palatino"], "size": 14})


PI = np.pi


def plot_options_l():
    typeList = [
        "bx-",
        "gp-",
        "rs-",
        "co-",
        "mD-",
        "yH-",
        "k*-",
        "gp--",
        "rs--",
        "co--",
        "mD--",
        "yH--",
        "k*--",
    ]
    return typeList


def plot_options(n=0, use_color=False):
    d = [
        {"c": "b", "linestyle": "-", "marker": "x", "markevery": 11},
        {"c": "g", "linestyle": "-", "marker": "p", "markevery": 13},
        {"c": "r", "linestyle": "-", "marker": "s", "markevery": 17},
        {"c": "m", "linestyle": "-", "marker": "o", "markevery": 15},
        {"c": "y", "linestyle": "-", "marker": "D", "markevery": 19},
        {"c": "k", "linestyle": "-", "marker": "^", "markevery": 21},
        {"c": "k", "linestyle": "-", "marker": "<", "markevery": 23},
    ]
    opt = d[n % len(d)]
    if not use_color:
        opt["c"] = "k"
    return opt


def get_default_extensions():
    return ["svg", "eps", "png"]


def generate_figure_filenames(imageName, extensions=get_default_extensions()):
    if isinstance(extensions, (str,)):
        extensions = [extensions]
    return [imageName + e if e.startswith(".") else imageName + "." + e for e in extensions]


def pow2(X):
    return X * X


def pow3(X):
    return X * X * X


def pow4(X):
    return X * X * X * X


def pow5(X):
    return X * X * X * X * X


def pow6(X):
    return X * X * X * X * X * X


def psd_pierson_moskowitz(W, V):
    g = 9.81
    A = 0.0081 * g * g
    B = 0.74 * pow4(g / V)
    psd = A / pow5(W) * exp(-B / pow4(W))
    return psd


def psd_jonswap(W, Hs, Tp, gamma=3.3):
    W0 = 2.0 * PI / Tp
    sigma_a = 0.07
    sigma_b = 0.09
    W = np.array(W)
    sigma = np.zeros(W.size)
    sigma[W <= W0] = sigma_a
    sigma[W > W0] = sigma_b
    alpha = pow4(W0 / W)
    Awm5 = 5.0 / 16.0 * alpha / W * Hs * Hs
    Bwm4 = 1.25 * alpha
    coeff = 1 - 0.287 * log(gamma)
    r = exp(-0.5 * pow2((W - W0) / (sigma * W0)))
    psd = coeff * Awm5 * exp(-Bwm4) * pow(gamma, r)
    return psd


def psd_ochi(W, Hs, Tp, ochiLambda):
    W0 = 2.0 * PI / Tp
    alpha = pow4(W0 / W)
    l414 = ochiLambda + 0.25
    psd = (
        0.25
        * pow(l414 * alpha, ochiLambda)
        / (W * math.gamma(ochiLambda))
        * (Hs * Hs)
        * exp(-l414 * alpha)
    )
    return psd


def psd_bretschneider(W, Hs, Tp):
    W0 = 2.0 * PI / Tp
    alpha = pow4(W0 / W)
    psd = 0.3125 * (alpha / W) * (Hs * Hs) * exp(-1.25 * alpha)
    return psd


def psd_ittc(W, Hs):
    g = 9.81
    A = 0.0081 * g * g
    B = 3.11 / (Hs * Hs)
    psd = A / pow5(W) * exp(-B / pow4(W))
    return psd


def psd_vignat_bovis(W, Hs, Tp):
    TpW5 = pow5(Tp * W)
    psd = 4001.26 * pow2(Hs) / (TpW5 * W) * exp(-11751.15 / TpW5)
    return psd


def psd_gaussian(W, Hs, Tp, sigma):
    W0 = 2.0 * PI / Tp
    A = 0.0625 * Hs * Hs / (sigma * pow(2 * PI, 1.5))
    psd = A * exp(-pow2(W - W0) / (8.0 * pow2(PI * sigma)))
    return psd


def image_pierson_moskowitz(extensions=get_default_extensions(), use_color=False):
    dw = 0.01
    W = np.arange(dw, 3 + dw, dw)
    psd07 = psd_pierson_moskowitz(W, 7)
    psd10 = psd_pierson_moskowitz(W, 10)
    psd15 = psd_pierson_moskowitz(W, 15)
    fig = plt.figure()
    plt.grid()
    plt.plot(W, psd07, label=r"$V= 7m/s$", **plot_options(0, use_color))
    plt.plot(W, psd10, label=r"$V=10m/s$", **plot_options(1, use_color))
    plt.plot(W, psd15, label=r"$V=15m/s$", **plot_options(2, use_color))
    plt.legend(loc="right")
    plt.xlabel(r"$\omega$ (rad/s)")
    plt.ylabel(r"Power Spectral Density ($m^2.s$)")
    plt.title(r"Pierson-Moskowitz spectrum")
    [plt.savefig(f) for f in generate_figure_filenames("spectrumPiersonMoskowitz", extensions)]
    plt.close(fig)


def image_jonswap(Hs=1, Tp=10, extensions=get_default_extensions(), use_color=False):
    dw = 0.01
    W = np.arange(dw, 3 + dw, dw)
    psd2 = psd_jonswap(W, Hs, Tp, 2)
    psd3 = psd_jonswap(W, Hs, Tp, 3)
    psd7 = psd_jonswap(W, Hs, Tp, 7)
    fig = plt.figure()
    plt.grid()
    plt.plot(W, psd2, label=r"$\gamma=2$", **plot_options(0, use_color))
    plt.plot(W, psd3, label=r"$\gamma=3$", **plot_options(1, use_color))
    plt.plot(W, psd7, label=r"$\gamma=7$", **plot_options(2, use_color))
    plt.legend(loc="right")
    plt.xlabel(r"$\omega$ (rad/s)")
    plt.ylabel(r"Power Spectral Density ($m^2.s$)")
    plt.title(r"JONSWAP spectrum - $H_s=" + str(Hs) + "m$ - $T_p=" + str(Tp) + "s$")
    [plt.savefig(f) for f in generate_figure_filenames("spectrumJonswap", extensions)]
    plt.close(fig)


def image_ochi(Hs=1, Tp=10, extensions=get_default_extensions(), use_color=False):
    dw = 0.01
    W = np.arange(dw, 3 + dw, dw)
    psd05 = psd_ochi(W, Hs, Tp, ochiLambda=0.5)
    psd15 = psd_ochi(W, Hs, Tp, ochiLambda=1.5)
    psd30 = psd_ochi(W, Hs, Tp, ochiLambda=3.0)
    fig = plt.figure()
    plt.grid()
    plt.plot(W, psd05, label=r"$\lambda=0.5$", **plot_options(0, use_color))
    plt.plot(W, psd15, label=r"$\lambda=1.5$", **plot_options(1, use_color))
    plt.plot(W, psd30, label=r"$\lambda=3.0$", **plot_options(2, use_color))
    plt.legend(loc="right")
    plt.xlabel(r"$\omega$ (rad/s)")
    plt.ylabel(r"Power Spectral Density ($m^2.s$)")
    plt.title(r"Ochi spectrum - $H_s=" + str(Hs) + "m$ - $T_p=" + str(Tp) + "s$")
    [plt.savefig(f) for f in generate_figure_filenames("spectrumOchi", extensions)]
    plt.close(fig)


def image_bretschneider(Hs=1, Tp=None, extensions=get_default_extensions(), use_color=False):
    dw = 0.01
    W = np.arange(dw, 3 + dw, dw)
    fig = plt.figure()
    plt.grid()
    if Tp is None:
        psd05 = psd_bretschneider(W, Hs, Tp=5)
        psd10 = psd_bretschneider(W, Hs, Tp=10)
        psd15 = psd_bretschneider(W, Hs, Tp=15)
        plt.plot(W, psd05, label=r"$T_p=5s$", **plot_options(0, use_color))
        plt.plot(W, psd10, label=r"$T_p=10s$", **plot_options(1, use_color))
        plt.plot(W, psd15, label=r"$T_p=15s$", **plot_options(2, use_color))
    elif isinstance(Tp, (float, int)):
        psd = psd_bretschneider(W, Hs, Tp=Tp)
        plt.plot(W, psd, label=r"$T_p=" + str(Tp) + "s$", **plot_options(0, use_color))
    elif isinstance(Tp, (list, set, tuple)):
        for i, tp in enumerate(Tp):
            psd = psd_bretschneider(W, Hs, Tp=tp)
            plt.plot(W, psd, label=r"$T_p=" + str(tp) + "s$", **plot_options(i, use_color))
    plt.legend(loc="right")
    plt.xlabel(r"$\omega$ (rad/s)")
    plt.ylabel(r"Power Spectral Density ($m^2.s$)")
    plt.title(r"ISSC Bretschneider spectrum - $H_s=" + str(Hs) + "m$")
    [plt.savefig(f) for f in generate_figure_filenames("spectrumBretschneider", extensions)]
    plt.close(fig)


def image_ittc(Hs=None, extensions=get_default_extensions(), use_color=False):
    dw = 0.01
    W = np.arange(dw, 3 + dw, dw)
    fig = plt.figure()
    plt.grid()
    if Hs is None:
        psd05 = psd_ittc(W, Hs=0.5)
        psd10 = psd_ittc(W, Hs=1.0)
        psd15 = psd_ittc(W, Hs=1.5)
        plt.plot(W, psd05, label=r"$H_s=0.5m$", **plot_options(0, use_color))
        plt.plot(W, psd10, label=r"$H_s=1.0m$", **plot_options(1, use_color))
        plt.plot(W, psd15, label=r"$H_s=1.5m$", **plot_options(2, use_color))
    elif isinstance(Hs, (float, int)):
        psd = psd_ittc(W, Hs=Hs)
        plt.plot(W, psd, label=r"$H_s=" + str(Hs) + "m$", **plot_options(0, use_color))
    elif isinstance(Hs, (list, set, tuple)):
        for i, hs in enumerate(Hs):
            psd = psd_ittc(W, Hs=hs)
            plt.plot(W, psd, label=r"$H_s=" + str(hs) + "m$", **plot_options(i, use_color))
    plt.legend(loc="right")
    plt.xlabel(r"$\omega$ (rad/s)")
    plt.ylabel(r"Power Spectral Density ($m^2.s$)")
    plt.title(r"ITTC spectrum")
    [plt.savefig(f) for f in generate_figure_filenames("spectrumITTC", extensions)]
    plt.close(fig)


def image_vignat_bovis(Hs=None, Tp=10, extensions=get_default_extensions(), use_color=False):
    dw = 0.01
    W = np.arange(dw, 3 + dw, dw)
    fig = plt.figure()
    plt.grid()
    if Hs is None:
        psd05 = psd_vignat_bovis(W, Hs=0.5, Tp=Tp)
        psd10 = psd_vignat_bovis(W, Hs=1.0, Tp=Tp)
        psd15 = psd_vignat_bovis(W, Hs=1.5, Tp=Tp)
        plt.plot(W, psd05, label=r"$H_s=0.5m$", **plot_options(0, use_color))
        plt.plot(W, psd10, label=r"$H_s=1.0m$", **plot_options(1, use_color))
        plt.plot(W, psd15, label=r"$H_s=1.5m$", **plot_options(2, use_color))
    elif isinstance(Hs, (float, int)):
        psd = psd_vignat_bovis(W, Hs=Hs, Tp=Tp)
        plt.plot(W, psd, label=r"$H_s=" + str(Hs) + "m$", **plot_options(0, use_color))
    elif isinstance(Hs, (list, set, tuple)):
        for i, hs in enumerate(Hs):
            psd = psd_vignat_bovis(W, Hs=hs, Tp=Tp)
            plt.plot(W, psd, label=r"$H_s=" + str(hs) + "m$", **plot_options(i, use_color))
    plt.legend(loc="right")
    plt.xlabel(r"$\omega$ (rad/s)")
    plt.ylabel(r"Power Spectral Density ($m^2.s$)")
    plt.title(r"Vignat Bovis spectrum")
    [plt.savefig(f) for f in generate_figure_filenames("spectrumVignatBovis", extensions)]
    plt.close(fig)


def image_gaussian(Hs=1, Tp=10, sigma=None, extensions=get_default_extensions(), use_color=False):
    dw = 0.01
    W = np.arange(dw, 3 + dw, dw)
    fig = plt.figure()
    plt.grid()
    if sigma is None:
        psd001 = psd_gaussian(W, Hs, Tp, sigma=0.01)
        psd002 = psd_gaussian(W, Hs, Tp, sigma=0.02)
        psd004 = psd_gaussian(W, Hs, Tp, sigma=0.04)
        plt.plot(W, psd001, label=r"$\sigma=0.01$", **plot_options(0, use_color))
        plt.plot(W, psd002, label=r"$\sigma=0.02$", **plot_options(1, use_color))
        plt.plot(W, psd004, label=r"$\sigma=0.04$", **plot_options(2, use_color))
    elif isinstance(sigma, (float, int)):
        psd = psd_gaussian(W, Hs, Tp, sigma=sigma)
        plt.plot(W, psd, label=r"$\sigma=" + str(sigma) + "$", **plot_options(0, use_color))
    elif isinstance(sigma, (list, set, tuple)):
        for sig in sigma:
            psd = psd_gaussian(W, Hs, Tp, sigma=sig)
            plt.plot(W, psd, label=r"$\sigma=" + str(sig) + "$", **plot_options(0, use_color))
    plt.legend(loc="right")
    plt.xlabel(r"$\omega$ (rad/s)")
    plt.ylabel(r"Power Spectral Density ($m^2.s$)")
    plt.title(r"Gaussian spectrum - $H_s=" + str(Hs) + "m$ - $T_p=" + str(Tp) + "s$")
    [plt.savefig(f) for f in generate_figure_filenames("spectrumGaussian", extensions)]
    plt.close(fig)


def image_monochromatique(extensions=get_default_extensions(), use_color=False):
    fig = plt.figure()
    plt.grid()
    dt = 0.05
    A = 3.5
    t = np.arange(0.0, 20 + dt, dt)
    T = 10
    y = A * np.cos(-2 * np.pi / T * t - 0.3)
    plt.plot(t, y, label=r"", **{"c": "k", "linestyle": "-"})
    ax = plt.gca()
    ax.arrow(5, 0, 0, A - 0.25, head_width=0.5, head_length=0.25, fc="k", ec="k")
    ax.text(4, A / 2, "A")
    ax.arrow(9.5, A + 0.15, T, 0, head_width=0.25, head_length=0.25, fc="k", ec="k")
    ax.arrow(9.5 + T, A + 0.15, -T, 0, head_width=0.25, head_length=0.25, fc="k", ec="k")
    ax.text(9.5 + T / 2, 0.9 * A, "T")
    plt.xlabel(r"$t$ (s)")
    plt.ylabel(r"$\eta \left( t \right)$ ($m$)")
    plt.title("")
    [plt.savefig(f) for f in generate_figure_filenames("waveMonochromatique", extensions)]
    plt.close(fig)


def image_bichromatique(extensions=get_default_extensions(), use_color=False):
    fig = plt.figure()
    plt.grid()
    dt = 0.025
    A1, A2 = 3.5, 1.3
    t = np.arange(0.0, 100 + dt, dt)
    T1, T2 = 10, 17
    y = A1 * np.cos(-2 * np.pi / T1 * t - 0.3) + A2 * np.cos(-2 * np.pi / T2 * t - 0.7)
    plt.plot(t, y, label=r"", **{"c": "k", "linestyle": "-"})
    plt.xlabel(r"$t$ (s)")
    plt.ylabel(r"$\eta \left( t \right)$ ($m$)")
    plt.title("")
    [plt.savefig(f) for f in generate_figure_filenames("waveBichromatique", extensions)]
    plt.close(fig)


def image_comparison1(Hs=1, Tp=10, extensions=get_default_extensions(), use_color=False):
    dw = 0.01
    W = np.arange(dw, 5 + dw, dw)
    fig = plt.figure()
    plt.grid()
    plt.plot(
        W,
        psd_pierson_moskowitz(W, 7),
        label=r"Pierson-Moskovitz, $V=7m/s$",
        **plot_options(0, use_color)
    )
    plt.plot(W, psd_ittc(W, Hs), label=r"ITTC, $H_s={0}m$".format(Hs), **plot_options(1, use_color))
    plt.plot(
        W,
        psd_bretschneider(W, Hs, Tp),
        label=r"ISSC Bretschneider, $H_s={0}m$, $T_p={1}s$".format(Hs, Tp),
        **plot_options(2, use_color)
    )
    plt.plot(
        W,
        psd_jonswap(W, Hs, Tp, gamma=2),
        label=r"JONSWAP, $H_s={0}m$, $T_p={1}s$, $\gamma=2$".format(Hs, Tp),
        **plot_options(3, use_color)
    )
    plt.plot(
        W,
        psd_ochi(W, Hs, Tp, 1.5),
        label=r"Ochi, $H_s={0}m$, $T_p={1}s$, $\lambda=1.5$".format(Hs, Tp),
        **plot_options(4, use_color)
    )
    plt.plot(
        W,
        psd_vignat_bovis(W, Hs, Tp),
        label=r"Vignat-Bovis, $H_s={0}m$, $T_p={1}s$".format(Hs, Tp),
        **plot_options(5, use_color)
    )
    plt.plot(
        W,
        psd_gaussian(W, Hs, Tp, 0.02),
        label=r"Gauss, $H_s={0}m$, $T_p={1}s$, $\sigma=0.02$".format(Hs, Tp),
        **plot_options(7, use_color)
    )
    plt.legend(loc="upper right")
    plt.xlabel(r"$\omega$ (rad/s)")
    plt.ylabel(r"Power Spectral Density ($m^2.s$)")
    plt.title(r"Spectra comparison")
    [plt.savefig(f) for f in generate_figure_filenames("spectrumComparison1", extensions)]
    plt.close(fig)


def image_comparison2(Hs=1, Tp=10, extensions=get_default_extensions(), use_color=False):
    def plot_options(n=0, use_color=False):
        d = [
            {"c": "k", "linestyle": "-", "marker": "x", "markevery": 11},
            {"c": "k", "linestyle": "-", "marker": "s", "markevery": 13},
            {"c": "k", "linestyle": "-", "marker": "D", "markevery": 17},
        ]
        opt = d[n % len(d)]
        if not use_color:
            opt["c"] = "k"
        return opt

    dw = 0.01
    W = np.arange(dw, 4 + dw, dw)
    fig = plt.figure()
    plt.grid()
    plt.plot(
        W,
        psd_bretschneider(W, Hs, Tp),
        label=r"ISSC Bretschneider, $H_s={0}m$, $T_p={1}s$".format(Hs, Tp),
        **plot_options(0, use_color)
    )
    plt.plot(
        W,
        psd_vignat_bovis(W, Hs, Tp),
        label=r"Vignat-Bovis, $H_s={0}m$, $T_p={1}s$".format(Hs, Tp),
        **plot_options(1, use_color)
    )
    plt.legend(loc="upper right")
    plt.xlabel(r"$\omega$ (rad/s)")
    plt.ylabel(r"Power Spectral Density ($m^2.s$)")
    [plt.savefig(f) for f in generate_figure_filenames("spectrumComparison2", extensions)]
    plt.close(fig)


def generate_images(e, use_color=False, images=None):
    if images is None:
        images = get_list_of_all_possible_images()
    dict_fun = get_dict_fun()
    for image in images:
        image = os.path.splitext(image)[0]
        if image.startswith("spectrum"):
            image = image[len("spectrum") :]
        dict_fun[image](extensions=e, use_color=use_color)


def get_dict_fun():
    return {
        "PiersonMoskowitz": image_pierson_moskowitz,
        "Jonswap": image_jonswap,
        "Ochi": image_ochi,
        "Bretschneider": image_bretschneider,
        "ITTC": image_ittc,
        "VignatBovis": image_vignat_bovis,
        "Gaussian": image_gaussian,
        "Comparison1": image_comparison1,
        "Comparison2": image_comparison2,
        "Monochromatique": image_monochromatique,
        "Bichromatique": image_bichromatique,
    }


def get_list_of_all_possible_images():
    return list(get_dict_fun())


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Script to generate different images.")
    parser.add_argument(
        "-c",
        "--color",
        action="store_true",
        help="Option to use color. Default is false",
        default=False,
    )
    parser.add_argument(
        "-n",
        "--names",
        type=str,
        nargs="*",
        help="Name of the spectrum to generate. Default will be create all possible images",
    )
    args = parser.parse_args()
    generate_images("png", args.color, args.names)
    generate_images("svg", args.color, args.names)
