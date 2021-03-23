from __future__ import print_function
import sys
import platform
import zipfile
import subprocess
import glob
import argparse
import os

def warning(*objs):
    print("ERROR: ", *objs, file=sys.stderr)


shortname = {
             'Linux': 'linux',
             'Windows': 'win'
            }

def get_platform():
    architecture = '64' if sys.maxsize > 2**32 else '32'
    return shortname[platform.system()] + architecture


def get_command_line_arguments():
    parser = argparse.ArgumentParser(description='Generate an FMU component')
    parser.add_argument('yaml', nargs='+', help='List of YAML files')
    parser.add_argument('-s', nargs=1, dest='stl', help='STL file containing the ship\'s mesh')
    parser.add_argument('-o', nargs=1, required=True, dest='output', help='Name of generated FMU file')
    return parser.parse_args()

def rename_dll(original_name):
    if original_name == "libfmi_simulator.so":
        return "fmi_simulator.so"
    return original_name

def search_replace_in_file(filename, old, new):
    f = open(filename,'r')
    filedata = f.read()
    f.close()
    newdata = filedata.replace(old, new)
    f = open(filename,'w')
    f.write(newdata)
    f.close()

if __name__ == "__main__":
    args = get_command_line_arguments()
    yaml_files = ''.join(args.yaml)
    dll_files = glob.glob('*.so') + glob.glob('*.dll')
    bin_dir = 'binaries/' + get_platform()
    # Generate XML
    with open('modelDescription.xml', 'w') as xmlfile:
        subprocess.check_call(['./generate_fmi_xml', yaml_files], stdout=xmlfile)
    # Generate YML
    with open('simulator_conf.yml', 'w') as outfile:
        subprocess.check_call(['cat', yaml_files], stdout=outfile)

    # Generate ZIP
    with zipfile.ZipFile(args.output[0], 'w') as zipf:
        zipf.write('simulator_conf.yml', 'resources/simulator_conf.yml')
        os.remove('simulator_conf.yml')
        zipf.write('modelDescription.xml')
        os.remove('modelDescription.xml')
        if args.stl:
            zipf.write(args.stl[0], 'resources/' + args.stl[0])
        for f in dll_files:
            zipf.write(f, bin_dir + '/' + rename_dll(f))
