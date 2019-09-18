#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QVector3D>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

QJsonArray vectorToJson(const QVector3D &vector) {
  QJsonArray result;
  for (int dim = 0; dim < 3; dim++)
    result.append(QJsonValue(vector[dim]));
  return result;
}

class Face {
public:
    //Champs de la face
    std::vector<QVector3D> vertices;    //coins de la face
    QVector3D normal; //vecteur normale de la face
    float c; //couleur

    //Constructeur par défaut : sans coins, avec un vecteur normale nul et de couleur 0
    Face() {
        this->vertices = {};
        this->normal = {0,0,0};
        this->c =0;
    }
    //constructeur principal
    Face(int faceNumber, QVector3D position, float couleur) {
        float x = position.x();
        float y = position.y();
        float z = position.z();
        QVector3D vertice1,vertice2,vertice3, vertice4;
        switch(faceNumber) {
            case 1 :    // correspond au vecteur normale (1,0,0)
                this->normal = QVector3D(1,0,0);
                vertice1 = QVector3D(x+1,y+1,z+1);
                vertice2 = QVector3D(x+1,y,z+1);
                vertice3 = QVector3D(x+1,y,z);
                vertice4 = QVector3D(x+1,y+1,z);
                break;
            case 2 :    //correpond au vecteur normale (-1,0,0)
                this->normal = QVector3D(-1,0,0);
                vertice1 = QVector3D(x,y+1,z+1);
                vertice2 = QVector3D(x,y,z+1);
                vertice3 = QVector3D(x,y,z);
                vertice4 = QVector3D(x,y+1,z);
                break;
            case 3 :    //correspond au vecteur normale (0,1,0)
                this->normal = QVector3D(0,1,0);
                vertice1 = QVector3D(x,y+1,z);
                vertice2 = QVector3D(x+1,y+1,z);
                vertice3 = QVector3D(x+1,y+1,z+1);
                vertice4 = QVector3D(x,y+1,z+1);
                break;
            case 4 :    //correspond au vecteur normale (0,-1,0)
                this->normal = QVector3D(0,-1,0);
                vertice1 = QVector3D(x,y,z);
                vertice2 = QVector3D(x+1,y,z);
                vertice3 = QVector3D(x+1,y,z+1);
                vertice4 = QVector3D(x,y,z+1);
                break;
            case 5 :    //correspond au vecteur normale (0,0,1)
                this->normal = QVector3D(0,0,1);
                vertice1 = QVector3D(x,y,z+1);
                vertice2 = QVector3D(x+1,y,z+1);
                vertice3 = QVector3D(x+1,y+1,z+1);
                vertice4 = QVector3D(x,y+1,z+1);
                break;
            case 6 : //correspond au vecteur normale (0,0,-1)
                this->normal = QVector3D(0,0,-1);
                vertice1 = QVector3D(x,y,z);
                vertice2 = QVector3D(x+1,y,z);
                vertice3 = QVector3D(x+1,y+1,z);
                vertice4 = QVector3D(x,y+1,z);
                break;
            default : //face par défaut (on choisit la première)
                this->normal = QVector3D(1,0,0);
                vertice1 = QVector3D(x+1,y,z);
                vertice2 = QVector3D(x+1,y+1,z);
                vertice3 = QVector3D(x+1,y+1,z+1);
                vertice4 = QVector3D(x+1,y,z+1);
                break;
        }
        //On finit la construction de la face !
        this->vertices = {vertice1,vertice2,vertice3,vertice4};
        this->c = couleur;
    }

  //Conversion Face en objet Json
  QJsonObject toJson() const {
    QJsonArray vertices_json;
    for (const QVector3D &v : vertices)
      vertices_json.append(vectorToJson(v));
    QJsonObject result;
    result["vertices"] = vertices_json;
    result["normal"] = vectorToJson(normal);
    result["color"] = c;
    return result;
  }
};

//this is the function we coded that returns an object of type vector of Faces
std::vector<Face> pgm3DToFaces(const std::string &path) {
  std::vector<Face> result = {};

  /*
  Première étape : on récupère les données de l'image et on l'a transforme en tableau 3D
  ****************
  */
  //compteurs
  int i = 0, j = 0, k = 0;
  int sizeX = 0, sizeY = 0 , sizeZ = 0; //dimension de l'image
  float colorMax; //hauteur de la couleur
  std::string inputLine = "";
  std::ifstream infile(path); // input du chemin du fichier
  std::stringstream ss; //buffer

  getline(infile,inputLine);
  //Message d'erreur en cas de mauvais header
  if (inputLine != "PGM3D"){
        std::cerr << "mauvais format de fichier !" << std::endl;
        exit(EXIT_FAILURE);
    }

    //mise en place du buffer
    ss << infile.rdbuf();
    //on récupère les dimension de l'image
    ss >> sizeX >> sizeY >> sizeZ ;
    //on récupère la hauteur de couleur max
    ss >> colorMax ;
    //initialisation de l'array 3D
    float image3D[sizeX][sizeY][sizeZ];
    //insertion des éléments dans le tableau 3D
    for (k= 0 ; k< sizeZ ; k++) {
        for(j=0 ; j<sizeY ; j++) {
            for (i=0 ;i<sizeX ; i++) {
                ss >> image3D[i][j][k];
            }
        }
    }

    /*
    Deuxième étape : analyse des faces
    **************
    */
    Face newFace = Face();
    QVector3D position = QVector3D(0,0,0);
    float comparedColor = 0 ,currentColor = 0 ;
    float x=0,y=0,z=0;

    for (k= 0 ; k< sizeZ ; k++) {
        for(j=0 ; j<sizeY ; j++) {
            for (i=0 ; i <sizeX ; i++ ){
                    //mise a jours de la position (avec centrage en 0) et couleur du voxel
                    x = i- sizeX*0.5;
                    y = j- sizeY*0.5;
                    z = k- sizeZ*0.5;
                    position = QVector3D(x,y,z);
                    currentColor = image3D[i][j][k];

                    //face droit
                        //detection des bords
                    if (i==(sizeX-1)){
                        comparedColor = 0;
                    } else {
                        comparedColor = image3D[i+1][j][k];
                    }
                        //création d'une nouvelle face dans le cas ou il y a une différence de couleurs
                    if ((currentColor-comparedColor) != 0){
                        newFace = Face(1,position,currentColor);
                        result.push_back(newFace);
                    }
                    //face gauche
                    if (i == 0) {
                        comparedColor=0;
                    } else {
                        comparedColor = image3D[i-1][j][k];
                    }
                    if ((currentColor-comparedColor) != 0){
                        newFace = Face(2,position,currentColor);
                        result.push_back(newFace);
                    }
                    //face avant
                    if (j == (sizeY-1)) {
                        comparedColor = 0;
                    } else {
                        comparedColor = image3D[i][j+1][k];
                    }
                    if ((currentColor - comparedColor) != 0) {
                        newFace = Face(3,position,currentColor);
                        result.push_back(newFace);
                    }

                    //face arrière
                    if (j==0) {
                        comparedColor = 0;
                    } else {
                        comparedColor = image3D[i][j-1][k] ;
                    }
                    if ((currentColor - comparedColor) != 0) {
                        newFace = Face(4,position,currentColor);
                        result.push_back(newFace);
                    }
                    //face dessus
                    if (k == (sizeZ-1)) {
                        comparedColor = 0;
                    } else {
                        comparedColor = image3D[i][j][k+1];
                    }
                    if ((currentColor - comparedColor) != 0) {
                        newFace = Face(5,position,currentColor);
                        result.push_back(newFace);
                    }

                    //face dessous
                    if (k==0) {
                        comparedColor = 0;
                    } else {
                        comparedColor = image3D[i][j][k-1] ;
                    }
                    if ((currentColor - comparedColor) != 0) {
                        newFace = Face(6,position,currentColor);
                        result.push_back(newFace);
                    }
            }
        }
    }
  return result;
}

void usage(int argc, char **argv) {
  (void)argc;
  std::cerr << "Usage: " << argv[0] << " <input.pgm3d>" << std::endl;
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    usage(argc, argv);
  }
  std::vector<Face> faces = pgm3DToFaces(argv[1]);
  QJsonArray faces_json;
  for (const Face &t : faces) {
    faces_json.append(t.toJson());
  }
  QString json_str(QJsonDocument(faces_json).toJson());
  std::cout << json_str.toStdString() << std::endl;
}
