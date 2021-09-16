changequote(`{{', `}}')

## Tutoriel 14 : utilisation des états filtrés

Ce tutoriel explique comment utiliser les états filtrés dans
des modèles d'effort distant et comment les écrire dans les
fichiers de sortie d'xdyn.

### Configuration d'xdyn

```python echo=False, results='raw', name='tutorial_14_load_yaml'
yaml_data = load_yaml('tutorial_14_filtered_states.yml')
```

Pour utiliser des états filtrés, on les définis dans la section `filtered
states`, définie pour chaque corps au même niveau que les clefs `name` ou
`position of body frame relative to mesh` par exemple :

```python echo=False, results='raw', name='tutorial_14_filtered_states_yaml'
y = {'filtered states': yaml_data['bodies'][0]['filtered states']}
print_yaml(y)
```

On peut récupérer les états filtrés dans les fichiers de sortie d'xdyn à l'aide
de la section `output` suivante :

```python echo=False, results='raw', name='tutorial_14_outputs'
print_yaml(yaml_data['output'])
```

Le fichier complet est:

```python echo=False, results='raw', name='tutorial_14_print_full_yaml'
print_yaml(yaml_data)
```

Si un état ne figure pas dans la section `filtered states`, sa valeur
filtrée sera identique à sa valeur non filtrée.

Dans un fichier Python (nommé `filtered_force.py` dans cet exemple) on écrit :

```python evaluate=False, results='hidden'
include({{filtered_force.py}})
```

On écrit ensuite un fichier `docker-compose.yml` :

```yaml
include({{docker-compose-filtered-states.yml}})
```

On peut alors lancer la simulation comme suit :

```bash
CURRENT_UID=$(id -u):$(id -g) docker-compose up
```

La partie `CURRENT_UID=$(id -u):$(id -g)` sert simplement à ce que les
éventuels fichiers générés le soient avec les permissions de l'utilisateur
courant.

Les résultats de la simulation sont visibles dans le fichier `filtered_states.csv`:

```python echo=False, results='raw', name='tutorial_14_plots'
data = csv('filtered_states.csv')
xplot = prepare_plot_data(data, x='t', y='x(TestShip)', name='État x non filtré')
xfilteredplot = prepare_plot_data(data, x='t', y='x_filtered(TestShip)', name='État x filtré')
gx = cartesian_graph([xplot, xfilteredplot], x='t (s)', y='Cavalement (m)')
create_layout(graph=gx, title='Position x au cours du temps')
zplot = prepare_plot_data(data, x='t', y='z(TestShip)', name='État z non filtré')
zfilteredplot = prepare_plot_data(data, x='t', y='z_filtered(TestShip)', name='État z filtré')
gz = cartesian_graph([zplot, zfilteredplot], x='t (s)', y='Pillonement (m)')
create_layout(graph=gz, title='Position z au cours du temps')
```

On constate que :

- les valeurs `x_filtered` et `x` sont identiques (puisqu'aucun filtrage n'est
  défini pour ce degré de liberté)
- le filtrage est bien pris en compte en z
