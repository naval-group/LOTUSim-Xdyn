L'objectif de la présente note est de donner les étapes nécessaires à la
création d'un dépôt de code open source.

Ceci implique d'utiliser un dépôt de code public et
de préparer le dépôt pour les contributions extérieures.

# Pré-requis

On suppose que le code que l'on souhaite est géré en configuration en utilisant
[Git](https://git-scm.com/) car c'est l'outil de versionnement le plus utilisé
dans l'industrie logicielle, qu'il est souple (utilisation hors connexion,
gestion des branches...) et s'intègre avec de nombreux outils.


# Dépôt de code

## Choix d'un dépôt public

Pour rendre le code accessible, il faut le stocker sur un dépôt public. Le choix
s'est porté sur [GitLab](https://about.gitlab.com/), parce que SIREHNA en utilise
une instance en interne, qu'il est très populaire et propose une intégration avec
de nombreux outils (analyse statique de code, couverture de
tests, intégration et déploiement continus, etc.).

## Création d'un groupe GitLab SIREHNA

En utilisant [ce
guide](https://docs.gitlab.com/ee/user/group/),
on crée un groupe SIREHNA afin que les développeurs appartenant à la société
soient identifiés comme tels. Ce compte sera administré par l'administrateur
système de SIREHNA.

## Création ou association des comptes utilisateurs

Les contributeurs internes à SIREHNA peuvent soit [créer un compte sur GitLab](https://gitlab.com/users/sign_up)
et demander son association au groupe, soit (le cas échéant) demander
l'association de leur compte existant. Nous préconisons plutôt de
systématiquement créer un nouveau compte afin de bien distinguer les
contributions faites par le collaborateur pendant son temps de travail des
éventuelles contributions faites sur son temps libre.

# Préparation du dépôt

Avant de rendre le dépôt de code public, plusieurs étapes
sont nécessaires. Il faut notamment créer cinq fichiers à
la racine du dépôt :

- Fichier de licence : `LICENSE`
- Règles pour les rétro-contributions : `CONTRIBUTING.md`
- Page d'accueil du projet : `README.md`
- Description sommaire des derniers changements : `CHANGELOG`
- Un code de conduite pour la communauté : `CODE_OF_CONDUCT.md`

## Fichier de licence

La licence choisie est [Eclipse Public License 2.0](https://opensource.org/licenses/EPL-2.0) parce qu'elle est à la
fois très souple (ce qui facilite les contributions) et prémunie d'une publication
du code sous une licence différente.

Il faut reproduire le code de la licence dans un fichier `LICENSE` que l'on met à
la racine du dépôt.

## Règles pour les rétro-contributions

Afin de clarifier les règles pour les contributions externes, il convient de
créer un fichier `CONTRIBUTING.md` à la racine du projet. Le contenu et le
format de ce fichier sont assez libres, mais de bons exemples existent :

- [Règles de contribution du projet Atom](https://github.com/atom/atom/blob/master/CONTRIBUTING.md)
- [Règles de contribution du projet Ruby on Rails](https://github.com/rails/rails/blob/master/CONTRIBUTING.md)
- [Règles de contribution du projet opengovernment](https://github.com/opengovernment/opengovernment/blob/master/CONTRIBUTING.md)

## Page d'accueil du projet

La page d'accueil (ou `README`) donne une description sommaire du projet, à quoi
il sert, et comment l'utiliser. Un guide d'écriture d'une telle page est
disponible [ici](https://open-source-guide.18f.gov/making-readmes-readable/).

## Description sommaire des derniers changements

Ce fichier vient en complément de l'outil de gestion de configuration et donne
une vue synthétique des derniers changements, contrairement à l'outil de gestion
de configuration qui en donne le détail. Des règles pour créer un tel fichier
sont disponibles [ici](http://keepachangelog.com/fr/1.0.0/).

## Code de conduite

Afin de normaliser les règles d'échanges dans la communauté libre que l'on crée,
il est préférable d'utiliser un code de conduite. Ce code de conduite définit
les comportements acceptables et non-acceptables et les règles de bienséances.
Un exemple d'un tel code de conduite est le
[contributor covenant](https://www.contributor-covenant.org/).

## Upload sur GitLab

C'est l'étape la plus simple. Elle consiste simplement à lancer :

~~~~~~~~~~~ {.bash}
git remote add gitlab https://gitlab.com/sirehna_naval_group/sirehna/xdyn.git
~~~~~~~~~~~

Pour mettre à jour le dépôt Gitlab avec les modifications faites à SIREHNA il
suffit dès lors de faire :

~~~~~~~~~~~ {.bash}
git push gitlab
~~~~~~~~~~~

De même, pour récupérer les modifications distantes :

~~~~~~~~~~~ {.bash}
git fetch gitlab
~~~~~~~~~~~

