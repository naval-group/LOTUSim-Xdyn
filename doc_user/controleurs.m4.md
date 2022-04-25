changequote(`{{', `}}')

# Contrôleurs

## Principe

xdyn étant avant tout un simulateur hydrodynamique et hydrostatique temporel,
il n'a pas vocation à remplacer un outil de simulation générique tel que
Matlab, Simpy ou Open Modelica. Par conséquent, la création de lois de commande
et de filtres est hors de son périmètre. En revanche, il est possible de
contrôler certains de ses modèles d'effort. Les modèles d'effort qui peuvent
être contrôlés sont :

- les [modèles de manœuvrabilité](#mod%C3%A8les-de-man%C5%93uvrabilit%C3%A9)
- le [modèle d'hélices Wageningen série B](#h%C3%A9lices-wageningen-s%C3%A9rie-b)
- le [contrôleur de cap simplifié](file:///home/cady/projects/xdyn/doc_user/doc.html#contr%C3%B4leur-de-cap-simplifi%C3%A9)
- le [contrôleur de position simplifié](#contr%C3%B4leur-de-position-simplifi%C3%A9)
- le [modèle hélice + safran](#h%C3%A9lice-et-safran)
- le [modèle Kt(J) et Kq(J)](#mod%C3%A8le-ktj-kqj)
- les [modèles d'effort distant (gRPC)](#mod%C3%A8le-deffort-distant)

On peut contrôler des modèles d'effort de deux manières :

- en renseignant les commandes de façon statique dans la section `commands` du
  fichier YAML d'entrée
- en utilisant des contrôleurs internes ([PID](#r%C3%A9gulateur-pid)) ou [externes](#contr%C3%B4leurs-externes) (interface gRPC)

Ces deux méthodes sont détaillées ci-dessous.

### Commandes statiques

La section `commands` permet de définir de manière statique les commandes
utilisées par les modèles d'effort qui peuvent être contrôlés. Les commandes à
utiliser à chaque instant sont alors connues par xdyn lors du lancement de la
simulation. Voici un exemple de commandes pour le modèle hélice + safran dont
la description complète est [ici](#h%C3%A9lice-et-safran).

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
commands:
  - name: port side propeller
    t: [1,3,10]
    rpm: {unit: rpm, values: [3000, 3000, 4000]}
    P/D: {unit: 1, values: [0.7,0.7,0.8]}
  - name: starboard propeller
    t: [1,3,10]
    rpm: {unit: rpm, values: [3000, 3000, 4000]}
    P/D: {unit: 1, values: [0.7,0.7,0.8]}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La valeur renseignée dans `name` doit correspondre à l'identifiant utilisé dans
la section `external forces`. Pour chaque effort contrôlé (identifié par
`name`), on donne une liste d'instants (en secondes) puis, pour chaque
commande, les valeurs à ces instants. Il doit donc y avoir, pour chaque
commande, autant de valeurs qu'il y a d'instants. Entre deux instants, les
valeurs des commandes sont interpolées linéairement. On peut définir autant de
clef qu'on le souhaite : les clefs inutilisées sont simplement ignorées.

Au-delà de la dernière valeur de temps renseignée, la dernière valeur de chaque
commande est maintenue. Avant la première valeur de temps, on utilise la première
valeur de chaque commande. Ainsi, pour l'exemple présenté ci-dessus, pour toute
valeur de $`t\geq 10`$, alors rpm=4000. Pour $`t\leq 1`$, rpm=3000.

Ainsi, si l'on ne renseigne qu'une seule valeur, elle sera utilisée pour toute la
simulation peut importe la valeur de temps renseignée.


### Commandes issues des contrôleurs

Les commandes des efforts commandés peuvent aussi être obtenues de manière
dynamique comme la sortie de **contrôleurs**.

Un contrôleur est un système permettant d'atteindre une valeur de consigne et
de la maintenir malgré les perturbations externes.  Il a besoin de deux entrées
: une consigne et une mesure, qu'il compare pour calculer la commande.

Le champ `controllers` (facultatif) à la racine du yaml permet de définir les
paramètres permettant d'intégrer des contrôleurs à la simulation, qui vont
calculer les commandes dont ont besoin les efforts commandés.

Les seule clefs communes à tous les types de contrôleurs sont `type` (pour
choisir le type de contrôleur) et `dt` (pour renseigner le pas de temps du
contrôleur). Le pas de temps `dt` est supposé constant, sauf si `dt` vaut zéro,
auquel cas le contrôleur doit donner à chaque appel la date du prochain appel.
Hormis `type` et `dt`, chaque type de contrôleur peut posséder sa propre
paramétrisation.

Deux types de contrôleur sont actuellement implémentés
:

- le [régulateur `PID`](#r%C3%A9gulateur-pid)
- les [contrôleurs externes](#contr%C3%B4leurs-externes) (appelés via une interface gRPC)

On peut spécifier à la fois des commandes et des contrôleurs pour obtenir les
commandes nécessaires aux efforts commandés. Toutefois, pour chaque effort
commandé, chaque commande doit être définie une fois et une seule, soit
directement dans le champ `commands`, soit calculée par un contrôleur du champ
`controllers`.

### Consignes des contrôleurs

Les valeurs des **consignes des contrôleurs** sont spécifiées de manière
statique dans une section `setpoints` (facultative) à la racine du YAML.

On donne une liste d'instants (en secondes) puis, pour chaque consigne, les
valeurs à ces instants.  Il doit donc y avoir, pour chaque consigne, autant de
valeurs qu'il y a d'instants.  Entre deux instants, les valeurs des commandes
sont interpolées linéairement.  On peut définir autant de consignes qu'on le
souhaite : les consignes inutilisées sont simplement ignorées.

Au-delà de la dernière valeur de temps renseignée, la dernière valeur de chaque
consigne est maintenue. Avant la première valeur de temps, on utilise la
première valeur de chaque consigne. Ainsi, si l'on ne renseigne qu'une seule
valeur, elle sera utilisée pour toute la simulation peut importe la valeur de
temps renseignée.

On peut définir plusieurs listes d'instants différents.

Par exemple :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
setpoints:
    - t: [0, 500, 800, 1000]
      psi_co: {unit: deg, values: [30, 40, 50, 60]}
    - t: [0, 50]
      u_co: {unit: knot, values: [0, 1]}
      v_co: {unit: knot, values: [0, 1]}
      w_co: {unit: knot, values: [0, 1]}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Régulateur PID

Le [régulateur PID](https://en.wikipedia.org/wiki/PID_controller) délivre un signal de commande à partir de la
différence entre la consigne et la mesure (l'erreur).

Le correcteur PID agit de trois manières :

- action proportionnelle : l'erreur est multipliée par un gain Kp ;
- action intégrale : l'erreur est intégrée et divisée par un gain Ki ;
- action dérivée : l'erreur est dérivée et multipliée par un gain Kd.

Pour calculer une commande en utilisant un régulateur PID, on ajoute une
section YAML à la section `controllers`. Par exemple, voici un YAML spécifiant
un contrôleur PID calculant la commande attendue pour asservir l'angle de barre
d'un modèle [hélice + safran](#h%C3%A9lices-wageningen-s%C3%A9rie-b) à un cap
commandé `psi_co` :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
controllers:
  - name: port side propeller
    type: PID
    dt: 1
    state weights:
      psi: 1
    setpoint: psi_co
    command: port side propeller(beta)
    gains:
      Kp: -1
      Ki: 0
      Kd: -1
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- `name` est utilisé pour identifier le contrôleur, notamment s'il génère des sorties supplémentaires
- `type` est le type de contrôleur (non-sensible à la casse) et vaut soit `grpc`, soit `pid`
- `dt` est la durée entre deux appels du contrôleur
- `state weights`, définit la mesure dont le contrôleur aura besoin, spécifiée par une formule linéaire permettant
  d'obtenir une valeur à partir des états du système lors de la simulation. On renseigne une liste de clefs/valeurs où les clefs
  correspondent au nom de l'état et les valeurs sont les coefficients. Un état non spécifié a pour coefficient 0.
  Les noms d'états valides sont : `x`, `y`, `z`, `u`, `v`, `w`, `p`, `q`, `r`, `qr`, `qi`, `qj`, `qk`, `phi`, `theta` et `psi`.
  Par exemple, pour obtenir `x / 2 - y` :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
state weights:
    x: 0.5
    y: -1
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- `gains`, contient trois champs `Kp`, `Ki` et `Kd` correspondant respectivement aux gains proportionnel, intégral et dérivé.
- `setpoint`, qui contient le nom de la consigne dont le contrôleur aura besoin,
   qu'il cherchera dans la section [`setpoints`](#consignes-des-contr%C3%B4leurs) décrite précédemment.
   Par example: `psi_co`.
- `command`, qui contient le nom complet de la commande que calcule le contrôleur,
   composé du nom du modèle d'effort concaténé avec le nom de la commande entre parenthèses.
   Par example: `PropRudd(rpm)`, `port side propeller(P/D)`.

### Contrôleurs externes

xdyn peut appeler des contrôleurs externes via une interface gRPC. Il est donc possible
d'utiliser un contrôleur existant, pour peu qu'une interface gRPC lui soit
adjointe. La paramétrisation de ces contrôleurs nécessite, _a minima_, un nom,
un type, un pas de temps et une URL. Par défaut, tous les contrôleurs d'xdyn
sont supposés à pas constant, à moins que `dt` soit pris égal à zéro, auquel
cas le contrôleur doit retourner à chaque appel la date du prochain (et peut
donc être à pas variable).

Un contrôleur externe n'ayant aucun paramètre pourrait donc être utilisé grâce
à la section YAML suivante :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
controllers:
  - name: some name
    type: grpc
    dt: 0.01
    url: localhost:9002
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cependant, les contrôleurs ont en général besoin de paramètres (les gains
notamment). C'est pourquoi il est possible de spécifier des paramètres
supplémentaires dans le YAML. Ces paramètres ne seront pas interprêtés par xdyn
et le noeud YAML complet (avec tous ses enfants) sera envoyé tel quel au
contrôleur. Voici un example de YAML pour utiliser un PID externe nécessitant
les mêmes entrées YAML que le PID interne d'xdyn :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
controllers:
  - name: some name
    type: grpc
    url: localhost:9002
    dt: 1
    state weights:
      psi: 1
    setpoint: psi_co
    command: port side propeller(beta)
    gains:
      Kp: -1
      Ki: 0
      Kd: -1
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Dans cet exemple, lors de l'initialisation du contrôleur externe, la chaîne
YAML suivante sera envoyée au contrôleur externe :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
name: some name
type: grpc
url: localhost:9002
dt: 1
state weights:
  psi: 1
setpoint: psi_co
command: port side propeller(beta)
gains:
  Kp: -1
  Ki: 0
  Kd: -1
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

L'interface gRPC utilisée par les contrôleurs est définie de la façon suivante (syntaxe .proto) :

~~~~{.protobuf}
include({{controller.proto}})
~~~~

Lors de l'initialisation d'xdyn, la méthode `set_parameters` de chaque contrôleur est appelée avec
le noeud YAML complet correspondant à ce contrôleur et la date t0 du premier isntant de simulation.
Le contrôleur externe doit répondre en donnant :

- la date à laquelle il doit être appelé pour la première fois (en général t0) : `date_of_first_callback`
- les consignes `setpoint_names` qu'xdyn doit lui fournir (lues depuis la section `setpoints` du fichier YAML d'xdyn)
- la représentation `angle_representation` que le contrôleur doit fournir (quaternions ou angles d'Euler)

Après l'initialisation, le solveur d'xdyn appelle le contrôleur externe à la date `date_of_first_callback`.
La méthode gRPC exacte appelée par xdyn dépend de la valeur de `angle_representation` retournée
par le contrôleur lors de l'appel à `set_parameters` :

- Si `set_parameters` vaut "QUATERNION" alors `get_commands_quaternion` sera appelée
- Si `set_parameters` vaut "EULER_321" alors `get_commands_euler_321` sera appelée

Dans tous les cas, xdyn fournit au contrôleur les états navire et leur dérivée, ainsi que les
consignes du contrôleur. La différence entre les deux méthodes est la représentation de l'attitude
du navire.

Le contrôleur répond en donnant la valeur des commandes qu'il calcule ainsi que la date à laquelle
il doit être rappelé. Le contrôleur peut inclure dans sa réponse des variables qui ne sont pas utilisées
dans les modèles d'effort : elles pourront alors apparaître dans la section `output` du fichier YAML
pour être sérialisées.

Le [tutoriel 11](#tutoriel-11-utilisation-dun-contr%C3%B4leur-distant) donne un exemple d'utilisation de contrôleur distant.

### Commandes lues d'un fichier CSV

Il est possible, à la place d'un contrôleur, de lire directement les sorties de
contrôleurs (les commandes actionneurs) depuis un fichier CSV. Pour ce faire,
on utilise le contrôleur `csv`. Ce dernier lit le fichier CSV ligne à ligne (il
ne charge pas l'ensemble du fichier en mémoire) et utilise les valeurs lues en
tant que commandes. La première ligne de ce fichier est sensée contenir les
noms des colonnes (qui doivent tous être distincts).

Sa paramétrisation est la suivante :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
controllers:
  - name: some unique name
    type: csv
    path: path/to/file.csv
    time column name: t
    separator: comma
    shift time column to match tstart: true
    commands:
        port side propeller(beta): beta_co
        port side propeller(rpm): rpm_co
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- `name` est utilisé pour identifier les sorties du contrôleur (cf. section
  `outputs`).
- `type` doit avoir pour valeur `csv` pour lire les commandes d'un fichier.
- `path` est le chemin (absolu ou relatif au répertoire de lancement d'xdyn)
  vers le fichier CSV. Le fichier CSV doit rester disponible pendant toute la
  durée de la simulation.
- `time column name` est l'identifiant de la colonne contenant les dates. Ces
  dates doivent être strictement croissantes.
- `separator` est le caractère utilisé pour séparer les colonnes (`comma` ou `semicolon`).
- `shift time column to match tstart` détermine l'origine des temps : si cette
  valeur est `true`, les valeurs de la colonne des temps correspondront
  précisément au temps de simulation. Si sa valeur est `false`, seul l'intervalle
  de temps entre deux lignes consécutives sera conservé : le premier instant
  sera supposé égal au début de la simulation, quelle que soit la valeur lue
  dans le CSV. Ceci permet de facilement réutiliser une partie d'un fichier CSV
  dans une simulation sans devoir changer toutes les dates.
- `commands` contient une liste de clefs-valeurs : la clef correspond à la commande
  actionneur retournée par ce contrôleur (sa sortie) et la valeur doit être un nom
  de colonne valide. Pour permettre plus de flexibilité, xdyn ne vérifie pas
  que la commande est bien utilisée.

Les chaînes de caractères n'ont pas besoin d'être entre guillemets (et s'il y a
des guillemets, ils doivent également figurer, le cas échéant, dans `time
column name` et `commands`.

Pour être interprétées comme des nombres flottants par xdyn, les valeurs lues dans le CSV
peuvent commencer un caractère (optionnel) `+` ou `-` suivi :

- **soit** d'une séquence de chiffres, contenant au plus un caractère décimal (`.`)
  et optionnellement suivi d'un exposant (un caractère `e` ou `E` suivi d'un
  signe optionnel et d'une séquence de chiffres)
- **soit** d'un préfix `0x` ou `0X` et d'une séquence de chiffres hexadécimaux avec
  au plus un caractère décimal (`.`) et optionnallement suivi d'un exposant (un
  caractère `p` ou `P` suivi d'un signe optionnel et d'une séquence de chiffres
  hexadécimaux)
- **soit** `INF` ou `INFINITY` (en ignorant la casse)
- **soit** `NAN` or `NANsequence` (en ignorant la casse) ou `sequence` est une
  séquence de caractères, chaque caractère étant un caractère alphanumérique ou
  le caractère (`_`) A valid floating point number for strtod using the "C"
  locale is formed by an optional sign character (+ or -), followed by one of:

Si les dates du fichier ne couvrent pas tout le fichier, la valeur des
commandes est de zéro avant la première valeur du CSV et est égale à la
dernière valeur lue après la dernière date du CSV.

