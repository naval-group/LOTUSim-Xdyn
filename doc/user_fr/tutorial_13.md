## Tutoriel 13 : utilisation des résultats de code potentiel avec les modèles d'effort distant (gRPC)

Ce tutoriel explique comment utiliser les résultats des codes potentiel (format HDB ou PRECAL_R)
dans des modèles d'effort gRPC.

### Configuration d'xdyn

```python echo=False, results='raw', name='tutorial_13_load_yaml'
yaml_data = load_yaml('tutorial_13_hdb_force_model.yml')
yaml_data['output'][0]['data'] = yaml_data['output'][0]['data'][1:3]
```

Pour utiliser un fichier HDB ou PRECAL_R, il faut ajouter la clef (optionnelle)
`hdb` (ou, respectivement, `raodb`) à la section d'effort gRPC:

```python echo=False, results='raw', name='tutorial_13_grpc_yaml'
print_yaml(yaml_data['bodies'][0]['external forces'][0])
```

Le fichier complet est:

```python echo=False, results='raw', name='tutorial_13_print_full_yaml'
print_yaml(yaml_data)
```

xdyn ne garantit pas que les fichiers HDB ou PRECAL_R utilisés dans le
YAML sont cohérents : il est tout à fait possible d'utiliser des fichiers
différents pour les efforts internes et pour les efforts externes.

Dans le modèle d'effort (ici un code Python), on peut écrire :

```python
class HDBForceModel(force.Model):
    """Outputs data from HDB in extra_observations."""

    def __init__(self, _, body_name, pot):
        """Initialize parameters from gRPC's set_parameters."""
        self.body_name = body_name
        self.pot = pot

    def get_parameters(self):
        """Parameter k is stiffness and c is damping."""
        return {'max_history_length': 0, 'needs_wave_outputs': False,
                'frame': self.body_name, 'x': 0, 'y': 0, 'z': 0, 'phi': 0,
                'theta': 0, 'psi': 0, 'required_commands': []}

    def force(self, states, _, __):
        """Force model."""
        extra_observations = {}
        extra_observations['Ma(0,0)'] = self.pot.Ma[0][0]
        return {'Fx': 0,
                'Fy': 0,
                'Fz': 0,
                'Mx': 0,
                'My': 0,
                'Mz': 0,
                'extra_observations': extra_observations }


if __name__ == '__main__':
    force.serve(HDBForceModel)
```

Les données sont fournies une seule fois, lors de l'initialisation du modèle,
dans le troisième paramètre du constructeur du modèle d'effort. Toutes les
données sont stockées dans des types Numpy (`ndarray`) afin de simplifier et
accélérer les traitements numériques.

On peut écrire ces informations dans les fichiers de sortie d'xdyn à l'aide
de la section `output` suivante :

```python echo=False, results='raw', name='tutorial_13_outputs'
print_yaml(yaml_data['output'])
```
