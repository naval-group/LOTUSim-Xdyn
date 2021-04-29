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
contrôleur, qui est donc constant) : chaque type de contrôleur possède sinon sa
propre paramétrisation. Deux types de contrôleur sont actuellement implémentés
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

On peut définir plusieurs liste d'instants différents.

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
d'utiliser un contrôleur existant, pour peu qu'une interface gRPC lui soit adjointe. La
paramétrisation de ces contrôleurs nécessite, _a minima_, un nom, un type, un
pas de temps (tous les contrôleurs d'xdyn sont à pas constant) et une URL. Un
contrôleur externe n'ayant aucun paramètre pourrait donc être utilisé grâce à
la section YAML suivante :

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

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.proto}
service Controller
{
    // Initialize the controller with YAML parameters
    rpc set_parameters(SetParametersRequest)                 returns (SetParametersResponse);
    // Calculate the commands using quaternions instead of angles
    rpc get_commands_quaternion(ControllerRequestQuaternion) returns (ControllerResponse);
    // Calculate the commands using angles in Rned2body = Rz(ψ).Ry(θ).Rx(ϕ) rotation convention
    rpc get_commands_euler_321(ControllerRequestEuler)       returns (ControllerResponse);
    // Return the extra observations this controller may give
    rpc get_extra_observations(ExtraObservationsRequest)     returns (ExtraObservationsResponse);
}

message ExtraObservationsRequest
{
}

message ExtraObservationsResponse
{
    map<string, double> extra_observations = 1; // Extra values that the controller can output (for logging & tracing purposes)
}

message SetParametersRequest
{
    string parameters = 1; // YAML string containing the parameters to set for this particular model. Extracted verbatim from xdyn's YAML file
    double t0         = 2; // Date of the beginning of the simulation (first timestep) in seconds
}

message SetParametersResponse
{
    enum AngleRepresentation {
        QUATERNION = 0;
        EULER_321  = 1;
    }
    double              date_of_first_callback = 1; // Date at which the controller should be called for the first time. Will often be equal to just t0.
    repeated string     setpoint_names         = 2; // Name of the controller inputs (setpoints) which xdyn must supply.
    AngleRepresentation angle_representation   = 3; // Does the controller need to be called with get_commands_quaternion or with get_commands_euler_321?
    bool                has_extra_observations = 4; // If set to true, the controller's get_extra_observations will be called.
    double              dt                     = 5; // Constant step of the controller. Only taken into account if dt>0: if dt==0, the controller is assumed to be a variable step controller and has to give the date at which the solver should call it again in ControllerResponse (next_call >= 0).
}

message ControllerRequestQuaternion
{
    ControllerStatesQuaternion states     = 1; // Ship states
    ControllerStatesQuaternion dstates_dt = 2; // Ship state derivatives (at the previous time step)
    repeated double            setpoints  = 3; // Controller inputs (setpoints). Must have the same size as setpoint_names in SetParametersResponse.
}

message ControllerRequestEuler
{
    ControllerStatesEuler states     = 1; // Ship states history
    ControllerStatesEuler dstates_dt = 2; // Ship state derivatives history
    repeated double       setpoints  = 3; // Controller inputs (setpoints). Must have the same size as setpoint_names in SetParametersResponse.
}

message ControllerStatesQuaternion
{
    double t  = 1;  // Simulation time (in seconds).
    double x  = 2;  // Projection on axis X of the NED frame of the vector between the origin of the NED frame and the origin of the BODY frame
    double y  = 3;  // Projection on axis Y of the NED frame of the vector between the origin of the NED frame and the origin of the BODY frame
    double z  = 4;  // Projection on axis Z of the NED frame of the vector between the origin of the NED frame and the origin of the BODY frame
    double u  = 5;  // Projection on axis X of the BODY frame of the vector of the ship's speed relative to the ground (BODY/NED)
    double v  = 6;  // Projection on axis Y of the BODY frame of the vector of the ship's speed relative to the ground (BODY/NED)
    double w  = 7;  // Projection on axis Z of the BODY frame of the vector of the ship's speed relative to the ground (BODY/NED)
    double p  = 8;  // Projection on axis X of the BODY frame of the vector of the ship's rotation speed relative to the ground (BODY/NED)
    double q  = 9;  // Projection on axis Y of the BODY frame of the vector of the ship's rotation speed relative to the ground (BODY/NED)
    double r  = 10; // Projection on axis Z of the BODY frame of the vector of the ship's rotation speed relative to the ground (BODY/NED)
    double qr = 11; // Real part of the quaternion defining the rotation from the NED frame to the ship's BODY frame
    double qi = 12; // First imaginary part of the quaternion defining the rotation from the NED frame to the ship's BODY frame
    double qj = 13; // Second imaginary part of the quaternion defining the rotation from the NED frame to the ship's BODY frame
    double qk = 14; // Third imaginary part of the quaternion defining the rotation from the NED frame to the ship's BODY frame
}

message ControllerStatesEuler
{
    double t     = 1;  // Simulation time (in seconds).
    double x     = 2;  // Projection on axis X of the NED frame of the vector between the origin of the NED frame and the origin of the BODY frame
    double y     = 3;  // Projection on axis Y of the NED frame of the vector between the origin of the NED frame and the origin of the BODY frame
    double z     = 4;  // Projection on axis Z of the NED frame of the vector between the origin of the NED frame and the origin of the BODY frame
    double u     = 5;  // Projection on axis X of the BODY frame of the vector of the ship's speed relative to the ground (BODY/NED)
    double v     = 6;  // Projection on axis Y of the BODY frame of the vector of the ship's speed relative to the ground (BODY/NED)
    double w     = 7;  // Projection on axis Z of the BODY frame of the vector of the ship's speed relative to the ground (BODY/NED)
    double p     = 8;  // Projection on axis X of the BODY frame of the vector of the ship's rotation speed relative to the ground (BODY/NED)
    double q     = 9;  // Projection on axis Y of the BODY frame of the vector of the ship's rotation speed relative to the ground (BODY/NED)
    double r     = 10; // Projection on axis Z of the BODY frame of the vector of the ship's rotation speed relative to the ground (BODY/NED)
    double phi   = 11; // First Euler angle. Actual interpretation depends on rotation convention (and hence on the gRPC method called)
    double theta = 12; // Second Euler angle. Actual interpretation depends on rotation convention (and hence on the gRPC method called)
    double psi   = 13; // Third Euler angle. Actual interpretation depends on rotation convention (and hence on the gRPC method called)
}

message ControllerResponse
{
    map<string,double> commands = 1; // Commands computed by the controller
    double next_call            = 2; // Date at which the solver should call the controller again
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Lors de l'initialisation d'xdyn, la méthode `set_parameters` de chaque contrôleur est appelée avec
le noeud YAML complet correspondant à ce contrôleur et la date t0 du premier isntant de simulation.
Le contrôleur externe doit répondre en donnant :

- la date à laquelle il doit être appelé pour la première fois (en général t0) : `date_of_first_callback`
- les consignes `setpoint_names` qu'xdyn doit lui fournir (lues depuis la section `setpoints` du fichier YAML d'xdyn)
- la représentation `angle_representation` que le contrôleur doit fournir (quaternions ou angles d'Euler)
- si le contrôleur possède des valeurs à sérialiser en plus des commandes qu'il calcule (qui, elles,
  sont automatiquement sérialisées) : clef `has_extra_observations`

Après l'initialisation, le solveur d'xdyn appelle le contrôleur externe à la date `date_of_first_callback`.
La méthode gRPC exacte appelée par xdyn dépend de la valeur de `angle_representation` retournée
par le contrôleur lors de l'appel à `set_parameters` :

- Si `set_parameters` vaut "QUATERNION" alors `get_commands_quaternion` sera appelée
- Si `set_parameters` vaut "EULER_321" alors `get_commands_euler_321` sera appelée

Dans tous les cas, xdyn fournit au contrôleur les états navire et leur dérivée, ainsi que les
consignes du contrôleur. La différence entre les deux méthodes est la représentation de l'attitude
du navire.

Le contrôleur répond en donnant la valeur des commandes qu'il calcule ainsi que la date à laquelle
il doit être rappelé.


