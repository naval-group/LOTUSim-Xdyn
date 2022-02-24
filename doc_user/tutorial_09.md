## Tutoriel 9 : utilisation d'un modèle de houle distant

Ce tutoriel explique comment utiliser un modèle de houle externe
dans xdyn.

Nous utiliserons Docker compose pour lancer le client (xdyn) et le
serveur de houle. Ceci n'est pas obligatoire (on peut se passer de
Docker et Docker Compose pour faire fonctionner l'ensemble), mais
l'utilisation de Docker simplifie grandement la mise en oeuvre.

### Vue d'ensemble

Pour ce tutoriel, on a besoin :

- d'un modèle de houle
- d'une mise en donnée xdyn

Le modèle de houle peut être implémenté en Python. Afin de simplifier
sa mise en oeuvre, on peut utiliser le dépôt
[https://gitlab.sirehna.com/sirehna/demo_docker_grpc](https://gitlab.sirehna.com/sirehna/demo_docker_grpc) qui contient déjà un
exemple de serveur de houle en Python.

### Mise en donnée xdyn

Dans un fichier YAML (nommé `tutorial_09_gRPC_wave_model.yml` dans cet exemple) on écrit :

```python echo=False, results='raw', name='tutorial_09_load_yaml'
yaml_data = load_yaml('tutorial_09_gRPC_wave_model.yml')
```

```python echo=False, results='raw', name='tutorial_09_print_yaml'
print_yaml(yaml_data)
```

### Écriture du modèle de houle

Dans un fichier Python (nommé `airy.py` dans cet exemple), on écrit :

```python echo=False, evaluate=True, results='raw'
print("```python")
with open("/waves/airy.py", "r", encoding="utf-8") as fid:
    content = fid.read()
print(content)
print("```")
```

### Lancement de la simulation

On commence par récupérer l'exemple de modèle de houle :

```bash
git clone git@gitlab.sirehna.com:sirehna/demo_docker_grpc.git
```

On écrit ensuite un fichier `docker-compose.yml` :

```yaml
version: '3'
services:
  waves-server:
    build: waves_grpc/python_server
    entrypoint: ["/bin/bash", "/entrypoint.sh", "/work/airy.py"]
    working_dir: /work
    volumes:
    - .:/work
    - ./waves_grpc:/proto
  xdyn:
    image: sirehna/xdyn
    working_dir: /data
    entrypoint: xdyn tutorial_09_gRPC_wave_model.yml --dt 0.1 --tend 1 -o tsv
    volumes:
    - .:/data
    depends_on:
    - waves-server
```

On peut alors lancer la simulation comme suit :

```bash
docker-compose up
```

### Sans Docker

Si l'on utilise pas Docker, il faut lancer le serveur de houle manuellement:

```shell
python3 airy.py
```

Puis il faut éditer le fichier YAML d'entrée de xdyn en remplaçant :

```yaml
environment models:
  - model: grpc
    url: waves-server:50051
```

par

```yaml
environment models:
  - model: grpc
    url: localhost:50051
```

On peut alors lancer xdyn normalement :

```bash
./xdyn tutorial_09_gRPC_wave_model.yml --dt 0.1 --tend 1 -o tsv
```
