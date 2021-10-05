# v5.0.0 (2021-10-05)

- #53 Xdyn server still stays locked into errors
- #204 Date of first call to external controller does not correspond to tstart
- #92 Irregular waves separate number of frequential and directional rays
- #195 Homogeneisation des cles associees a precal_r dans fichiers yaml
- #200 Mistake in conversion matrix in doc
- #199 Bug cannot parse binary stl
- #52 Fix failed ci'
- #196 Bug can parse precal_r files with windows end of lines
- #51 Bump ssc version
- #138 Rendre l inclusion de maillage optionnelle dans les fichiers hdf5
- #50 Server modes error message is not reset when handling a new request
- #100 Ajouter le calcul a chaque instant des moyennes de tous les etats pour utilisation dans les
- #198 Corriger les typos dans la documentation suite a la relecture de code 118
- #197 Add hdb precal_r wave drift force data to grpc interface
- #40 Automatic detection of mesh panels orientation prevents the use of some geometries e g catamarans
- #41 Default outputting of all forces
- #46 Bad initialization of states for cs modes
- #49 Xdyn server stays locked into errors
- #194 Fire fix build issue on gitlab com
- #187 Add results from precal_r or hdb to grpc force interface
- #45 New feature mmg standard maneuvering force model
- #42 Co simulation json websocket mode returns the time vector for all states
- #193 Fix failing unit test for debian 9 for holtropmennenforcemodeltest numerical_example_1984
- #178 Precal r extraction des informations necessaires au calcul des efforts de froude krylov
- #191 Update current changelog md
- #188 Describe wave model conventions used by precal_r and aqua
- #177 Precal r extraction des matrices d amortissement de radiation
- #183 Slides for remote model training
- #184 Fix pulsation size check in hdb
- #175 Precal r extraction des informations necessaires au calcul des efforts de diffraction


# v4.3.0 (2021-06-29)

- #176 Precal r extraction des masses ajoutees
- #165 Mise en place de l interface precal_r x dyn
- #182 Fixed typo in doc_user interfaces_utilisateur m4 md
- #154 Rendre le developpement de modules modeles d efforts essentiellement possible facile par les
- #172 Should be able to output commands
- #95 Fix wave grpc
- #174 Add a command line flag to display concatenated yaml
- #89 Problem with history class
- #90 Add a word target for user documentation
- #39 Missing parameter documentation for radiation damping force model
- #37 Undocumented changes on diffraction force model
- #32 Diffraction force model does not take encounter frequency into account
- #31 Bug correction and performance improvement in radiationdampingforcemodel


# v4.2.0 (2021-05-04)

- #166 External controllers
- #16 New feature generic quadratic hydrodynamic polar model
- #171 Fix error outputting
- #28 Segfault in unit tests using googletest s testing internal capturestderr


# v4.1.0 (2021-04-16)

- #170 Remove local version of ssc solver
- #164 Pid controller for xdyn
- #5 Radiation damping force model improvement
- #17 New feature generic quadratic aerodynamic polar model
- #25 Holtrop mennen force model was not added to the simulator api
- #151 Integrate fmi generation
- #167 Bug sur le numero de version lors de l installation du paquet debian
- #22 Grpc server modes cs and me do not return the details of errors occurring in xdyn
- #21 Co simulation response always starts at t 0
- #7 Adding air specific mass to environment description
- #8 Outputting forces in server mode
- #4 Force models refactoring
- #6 Fix yaml rendering problem
- #1 Wind force models
- #155 Frequency discretisation of irregular wave spectrum in equal energy bins


# v4.0.4 (2020-11-20)

- #157 Push to gitlab
- #156 Reparer l integration continue de travis
- #153 Integration de la proposition de bug fix radiation damping force model correction
- #152 Add grpc interface to model exchange cosimulation
- #118 Divergence in tutorial_08_diffraction yml
- #120 Negative speed in resistance model in tutorial_05_froude_krylov yml
- #126 Modele propeller wageningen b series pris en compte repere
- #127 Definition des plages de temps et de valeur pour le forcage des degres de liberte
- #132 Bug dans la generation de la doc
- #134 Wave height in body frame does not change regardless of ship speed
- #109 Discontinuities found in euler angles
- #91 Bug on moment computation when efforts are expressed in a rotated frame
- #105 Upgrade gcc


# v4.0.3 (2020-03-16)



# v4.0.2 (2020-01-13)

- #99 Update spectrum images generation for documentation
- #94 Fixed docker compose tests for ci
- #85 Grpc force models do not register their commands
- #83 Update waves_grpc submodule not to depend on other submodules
- #82 Update repository submodule to fix github ci
- #77 Add the possibility to control precision of floating point numbers with ascii export
- #71 Preparer la serialisation de sorties supplementaires dans xdyn_for_cs
- #76 Ask the grpc force model for the reference frame
- #74 Create grpc docker container
- #72 Grpc force models
- #49 Vectorize orbital_velocity
- #55 Xdyn client code rpc wave model
- #50 Vectorize dynamic_pressure
- #69 Use const commands for force models
- #48 Vectorize elevation
- #68 Typo update in all hpp and cpp
- #67 Remove python dependencies from build
- #66 Fixed xdyn build without hos
- #65 Remove hos that will be refactored later
- #63 Build xdyn windows version with posix thread model
- #64 Improve makefile build scripts for eclipse


# v4.0.1 (2019-05-20)

- #62 Updated ci build for gitlab and travis
- #61 Update user documentation on tutorials
- #60 Fix readme travis ci authentication
- #59 Fixes for github
- #43 Add github deployment
- #42 Fixed for github travis
- #29 Fix issues on documentation detected by jjm
- #33 Simplify ssc fetching the ssc artifacts
- #31 Clean up deprecated build construction files
- #30 Websocket server for model exchange
- #30 Websocket server for model exchange
- #24 Get repository ready for github
- #23 Improve error message when yaml is invalid
- #20 Erreur lors de l utilisation du modele d effort constant force
- #14 Ajout module d effort constant
- #18 Les fichiers yaml d entrees ont une erreur d unite pour les tp sans consequence pour les resultats


