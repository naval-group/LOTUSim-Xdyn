## Tutoriel 12 : utilisation des fichiers de résultat de PRECAL_R

Ce tutoriel explique comment utiliser les fichiers générés par PRECAL_R
à la place (ou en complément) des fichiers HDB pour les masses ajoutées.

### Préparation : génération des fichiers de sortie PRECAL_R

Le flag 'calcAmasDampCoefInfFreq' doit avoir la valueur `true` dans le fichier
d'entrée de PRECAL_R (elle est à `false` par défaut). Elle est localisée dans
la section `sim` > `parHYD` > `calcAmasDampCoefInfFreq`. Pour plus de détails,
on pourra se référer au manuel théorique de PRECAL_R (sections 2.3 et 2.4) et à
son manuel utilisateur (section 3.3.2, p. 25).

### Configuration d'xdyn

Si le fichier de sortie de PRECAL_R s'appelle `ONRT_SIMMAN.raodb.ini`, on
utilise la section `bodies[0]/dynamics/added mass matrix at the center of
gravity and projected in the body frame` suivante :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
added mass matrix at the center of gravity and projected in the body frame:
    from PRECAL_R: ONRT_SIMMAN.raodb.ini
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Voici le fichier d'exemple complet :

```python echo=False, results='raw', name='tutorial_12_load_yaml'
yaml_data = load_yaml('tutorial_12_precal_r.yml')
print_yaml(yaml_data)
```

Dans le fichier de sortie de PRECAL_R, la section définissant la matrice de masse ajoutée à fréquence infinie a l'allure suivante :

```ini
[added_mass_damping_matrix_inf_freq]

total_added_mass_matrix_inf_freq_U1_mu1 = {
0.110E+06,-0.888E-01,0.226E+06,-0.144E+00,0.270E+08,0.551E+01
-0.122E-01,0.344E+07,-0.563E-02,-0.113E+07,0.157E+02,0.497E+08
0.227E+06,-0.898E+00,0.129E+08,0.763E+01,0.844E+08,0.130E+01
0.183E+00,-0.123E+07,0.251E+01,0.498E+08,0.104E+03,0.338E+09
0.270E+08,0.106E+01,0.845E+08,-0.431E+02,0.119E+11,-0.341E+03
0.164E+01,0.497E+08,0.101E+02,0.345E+09,-0.390E+03,0.522E+10
}
```
