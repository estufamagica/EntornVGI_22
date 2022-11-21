//******** PRACTICA VISUALITZACI� GR�FICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn b�sic VS2022 MULTIFINESTRA amb OpenGL 3.3+, interf�cie MFC, llibreries GLM i primitives GLUT en codi font
//******** Marc Vivet, Carme Juli�, D�bora Gil, Enric Mart� (Octubre 2022)
// fractals.cpp : Funcions de lectura i generaci� de fractals 


#include "StdAfx.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include "material.h"
#include "fractals.h"
#include <list>

//#include "EntornVGIView.h"

// -------------- Entorn VGI: VARIABLES utilitzades en els fractals
// FMAX: Index m�xim de la matriu d'al�ades. Definida a fractals.h (valor:512)
float zz[FMAX+1][FMAX+1];				// Matriu d'al�ades (i,j,zz[i][j]).
float normalsC[FMAX+1][FMAX+1][3];		// Matriu que guarda Vectors Normals per cares.
float normalsV[FMAX+1][FMAX+1][3];		// Matriu que guarda Vectors Normals per a v�rtexs.

// -------------- Entorn VGI: Valors m�xim i m�nim de Z necessaris per interpolar
//                            correctament la paleta de colors (iluminaci� suau del fractal)
float zmax=0; 
float zmin=0;

// -------------- Entorn VGI: Variables pics
float cx[6],cy[6],radi[6],hmax[6]; // Centres, radis i al�ades de les muntanyes
float a=1.0*FMAX*(0.65);			// Parametre lemniscata

// -------------- Entorn VGI: N�mero de pics
int npics=0;

float r;

// -------------- Entorn VGI: DEFINICI� DE LES FUNCIONS

// llegir_pts: Funci� de lectura d'un fitxer fractal amb pics.
// Variables:	- nomf: Nom del fitxer a llegir.
int llegir_pts(char *nomf)
{	int i,j,step;
	FILE *fd;
	i = 0;	j = 0;	step = 1; fd = NULL;

// 1. INICIALITZA MATRIUS zz i normals
for(i=0;i<=512;i=i++)
	{ for(j=0;j<=FMAX;j=j++)
		{	zz[i][j]=0.0;
			normalsC[i][j][0]=0.0;
			normalsC[i][j][1]=0.0;
			normalsC[i][j][2]=0.0;
			normalsV[i][j][0]=0.0;
			normalsV[i][j][1]=0.0;
			normalsV[i][j][2]=0.0;
		}
	}

// 2. INICIALITZA PICS
for(i=0;i<=6;i=i++)
	{ cx[i]=0; cy[i]=0; radi[i]=0; hmax[i]=0;
	}

// 3. CAL FER: OBRIR FITXER FRACTAL I LLEGIR ALC,ADES ASSIGNANT-LES
//    A LA MATRIU ZZ DE FORMA EQUIESPAIADA.
int counter_file = 0;
int counter_num = 0;
int counter_pic = 0;
//buff char[300];
fd = fopen(nomf, "r");
int n, m, total, numpic;
float x, y, rad, h;

//float* buff_alturas = new float[FMAX + 1 * FMAX + 1];

std::list<float> buff_alturas;


if (fd == NULL) perror("Error opening file");
else
{
	fscanf(fd, "%d %d \n", &n, &m);
	total = n * m;
	std::string line;
	
	char path_fractals[60] = "../../EntonrVGI/fractals/";

	//StrNCat(path_fractals, &nomf, 30);

	std::ifstream rd(nomf);
	step = FMAX/(n-1);
	
	size_t pos = 0;
	std::string delimiter = " ";
	std::string token;
	size_t count_token = 0;
	while (std::getline(rd, line))
	{
		//cout << prueba << "\n";
		if ((counter_file>0)&&(counter_file<total+1)) //alturas fichero
		{
			float aux = std::stof(line);
			//buff_alturas[counter_num]=aux;
			buff_alturas.push_back(aux);
			counter_num++;
		}
		//picos, centro radio i altura

		if (counter_file == total+1)
		{
			numpic = std::stoi(line);
		}

		if ((counter_file>total+1)&&(numpic!=0))
		{
			pos = 0;
			count_token = 0;
			while ((pos = line.find(delimiter)) != std::string::npos) {
				token = line.substr(0, pos);
				line.erase(0, pos + delimiter.length());

				switch (count_token)
				{
				
				case 0:
					
					cx[counter_pic] = std::stof(token);

					break;
				case 1:
					cy[counter_pic] = std::stof(token);

					break;
				case 2:
					radi[counter_pic] = std::stof(token);

					break;
				case 3:
					hmax[counter_pic] = std::stof(token);

					break;

				default:
					break;
				}

				count_token++;
			}
			//canf(fd, "%f %f %f %f\n", &x, &y, &rad, &h);

			//cx[counter_pic] = x; cy[counter_pic] = y; radi[counter_pic] = rad; hmax[counter_pic] = h;

			counter_pic++;
		}

		counter_file++;
	}

	

}


counter_num = 0;

for (int i = 0; i < FMAX + 1; i = i + step)
{
	for (int j = 0; j < FMAX + 1; j = j + step)
	{
		if (counter_num < total)
		{
			//zz[i][j] = buff_alturas[counter_num];
			zz[i][j] = buff_alturas.front();
			buff_alturas.pop_front();
			counter_num++;
		}
	}
}

// 4. CAL FER: LLEGIR EL NOMBRE DE PICS I ELS VALORS (CENTRE,RADI 
//    I AL�ADA M�XIMA.

// 5. INICIALITZAR LA VARIABLE ALEAT�RIA
srand( (unsigned) time(NULL)); //por mirar
r=(float) rand()/RAND_MAX;

// Funci� retorna el pas entre al�ades a la variable step, 
// calculat en funci� del nombre d'al�ades inicials i del
// tamany de la matriu.
return step;

}

// escriure_pts: Funci� d'escriptura en un fitxer de les al�ades i pics 
//               d'un fractal segons la resoluci� actual.
// Variables:	- nomf: Nom del fitxer a escriure.
//				- paso: Resoluci� del fractal a escriure.
bool escriure_pts(char *nomf,int paso)
{	int i,j;
	FILE *fd;

	i = 0;	j = 0;	fd = NULL;


// 1. OBRIR FITXER

// 2. ESCRIURE AL�ADES
// 2.1 Cap�alera n n segons la variable paso

// 2.2 Al�ades Z (una per l�nia)

// 3. ESCRIURE PICS
// 3.1 Escriure n�mero de pics <cr>
// 3.2 Escriure centre cx,cy, radi i al�ada dels pics (un pic per l�nia).

// retorna true si escritura s'ha realitzat amb �xit.
return true;

}

// itera_fractal: C�lcul de les al�ades intermitges.
// Variables: - bruit: Tipus de soroll (Linial,Quadr�tic,SQRT o diferenciable)
//            - paso: Pas d'iteraci� del fractal.
void itera_fractal(char bruit,int paso)
{
// 1. CAL FER: C�LCUL DE LES AL�ADES INTERMITGES DES DEL PAS paso/2 FINS A 1 SEGONS ALGORISME DEL PUNT MIG ALEATORI (Teoria, tema 6 Fractals)

// 2. CAL FER: CALCUL DEL MAXIM I MINIM DE LES AL�ADES INICIALS UN COP GENERADES TOTES LES AL�ADES (per l'assignaci� de colors de la paleta)

}


//soroll: Calcul del soroll en les alsades segons la dist�ncia
//        als picsdistancia.
// Variables: - i,j: Posici� de l'al�ada a calcular el soroll..
// 			  - alf: M�xim valor perm�s. En el nostre cas, el valor del
//                   de la variable pas que ens d�na la resoluci� del fractal.
//            - noise: Tipus de soroll (linial, quadr�tic,sqrt o diferenciable).
double soroll(int i,int j,double alf,char noise)
{
	double ff,s,r;

// C�lcul de la variable aleat�ria entre (0,1).
	r=(float) rand()/RAND_MAX;

// C�lcul del soroll segons el tipus.
	switch(noise)
	{
	case S_LINEAL:
		ff=soroll_lin(i,j);
		break;
	case S_QUADRATIC:
		ff=soroll_quad(i,j);
		break;
	case S_SQRT:
		ff=soroll_sq(i,j);
		break;
	case S_DIFERENCIABLE:
		ff=soroll_dif(i,j);
		break;
	default:
		ff=0;
	} 
	s=ff*r*alf;
//Retorn del valor del soroll.
	return s;	
}


//------------------ CALCUL DELS SOROLLS  --------------------/

// C�lcul del soroll linial segons la posici� del punt (x,y)
double soroll_lin(int x, int y)
{	double aux_sl = 0.0;

	return aux_sl;
}

// C�lcul del soroll quadr�tic segons la posici� del punt (x,y)
double soroll_quad(int x, int y)
{	double aux_sq = 0.0;

	return aux_sq;
}

// C�lcul del soroll arrel quadrada segons la posici� del punt (x,y)
double soroll_sq(int x, int y)
{	double aux_sq = 0.0;

	return aux_sq;
}

// C�lcul del soroll diferenciable segons la posici� del punt (x,y)
double soroll_dif(int x, int y)
{	double aux_sd = 0.0;

	return aux_sd;
}
//------------------ FI CALCUL DELS SOROLLS  --------------------/


// loadfractVAO: TRIANGULACIO DEL TERRENY per VAO(Vertex Array Objects) SENSE EBO (Element Buffer Objects)
// Par�metres: - palcolor: Booleana que indica si la coloraci� per paleta �s activa [TRUE] o el coklor del fractal �s el monocolor segons el color de l'objecte [FALSE].
//             - paleta: Tipus de paleta si palcolor �s TRUE. Pot agafar els valors de: MEDITERRANI, TUNDRA o GLACIAR.
//             - iluminaci�: Tipus d'ilumina'
//             - ref_mat: Booleana d'activaci� de la reflectivitat de material, una forma de donar color. Usualment, false.
//			   - sw_mat: Booleana d'activaci� dels diferents materials (emissi�[0], ambient[1], difusa[2], especular[3]),
//					[4]: booleana per a definir color segons el material (TRUE) o segons vector color del VAO -vector colors- (FALSE).
//					Par�metre a passar a funci� SeleccionaColorMaterial() del m�dul material.cpp
//			   - textur: Booleana d'activaci� per a texturar el fractal [TRUE] o no [FALSE]
//			   - step: Pas de recorregut per a formar els triangles. M�xim: el valor calculat adel fitxer fractal. Minim: 1.
//			   - vaoID: Identificador de VAO associat al fractal creat. S'utilitzar� posteriorment per a dibuixar el fractal (VAO)
//			   - nvertexs: Nombre de v�rtexs generat per a dibuixar el fractal. En funci� del pas de recoregut definit en el par�metre step.
CVAO loadfractVAO(bool palcolor, char paleta, char iluminacio, bool sw_mat[5], bool textur, int step)
{
	int i, j, index;
	float ctx, cty;
	CColor col_Vertex;								// Color del v�rtex del fractal.
	float paletaR[MAX_PALETA_FRACTAL], paletaG[MAX_PALETA_FRACTAL], paletaB[MAX_PALETA_FRACTAL];	// Paleta de colors

	i = 0;		j = 0;		index = 0;
	ctx = 0.0f;	cty = 0.0f;
	col_Vertex.r = 0.0;		col_Vertex.g = 0.0;		col_Vertex.b = 0.0;
	for (i = 0; i < MAX_PALETA_FRACTAL; i++) { paletaR[i] = 0.0f; paletaG[i] = 0.0f;	paletaB[i] = 0.0f; }

// VAO
	CVAO FractalVAO;
	FractalVAO.vaoId = 0;	FractalVAO.vboId = 0;	FractalVAO.nVertexs = 0;	// Valors inicialitzats per a VAO sense EBO.
	FractalVAO.eboId = 0;	FractalVAO.nIndices = 0;								// Valors inicialitzats per a VAO amb EBO.

	std::vector <double> vertices, normals, colors, textures;	// Definici� vectors din�mics per a vertexs, normals i textures 
	vertices.resize(0);		textures.resize(0);		colors.resize(0);	normals.resize(0);	// Reinicialitzar vectors

// Obtenir color actual definit en OpenGL amb glColor();
	GLfloat cColor[4];
	glGetFloatv(GL_CURRENT_COLOR, cColor);

	switch (paleta)
	{
	case MEDITERRANI:
		// CAL FER: Configurar paleta mediterrani

		break;
	case GLACIAR:
		// CAL FER: Configurar paleta glaciar

		break;

	case TUNDRA:
		// CAL FER: Configurar paleta tundra

		break;

	default:
		// CAL FER: Configurar paleta color blanc
		break;
	}

// CAL FER: C�lcul del valor de traslaci� per a centrar el fractal aplicat a tots els v�rtexs del fractal.
	const float centrat = FMAX / 2.f;

// 2. CAL FER:CREAR ELS V�RTEXS DELS TRIANGLES SEGONS EL PAS (step)
//				I DEFINIR ELS VECTORS NORMALS DE CADA V�RTEX EN FUNCI� DE
//				LA ILUMINACI� (valiable iluminacio)

	if ((iluminacio == GOURAUD) || (iluminacio == PHONG))
	{
// PER A IL.LUMINACI� GOURAUD CAL DESENVOLUPAR EL SEG�ENT CODI:

// CAL FER: Definir els "for" per a recorrer la matriu amb indexs (i,j)

// ------------------------------- Primer Triangle ------------------------------------------
		// -------- V�RTEX 1
		// VECTOR NORMAL
		normals.push_back(normalsV[i][j][0]);		normals.push_back(normalsV[i][j][1]);	normals.push_back(normalsV[i][j][2]); // Vector Normal
		// COLOR
		if (palcolor) {	//	Colorejar segons al�ada sobre la paleta de colors colorR,colorG,colorB
						// CAL FER: <C�LCUL index>;
						colors.push_back(paletaR[index]);	colors.push_back(paletaG[index]);		colors.push_back(paletaB[index]); colors.push_back(1.0);// Vector Colors
					}
		else {	//	Colorejar segons color de l'objecte (FET)
				colors.push_back(cColor[0]);	 colors.push_back(cColor[1]);		colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
			}
		//	Assignaci� TEXTURA
		if (textur) {	// CAL FER: <C�LCUL (ctx,cty)>
						textures.push_back(ctx); textures.push_back(cty);			// Vector Textures
		}
		else {	textures.push_back(0.0f); textures.push_back(0.0f);				// Vector Textures
			}
		// VERTEX
		vertices.push_back(i);	vertices.push_back(j);	vertices.push_back(zz[i][j]);	// Vector Vertices

		// -------- V�RTEX 2
		// VECTOR NORMAL
		normals.push_back(normalsV[i][j][0]);		normals.push_back(normalsV[i][j][1]);	normals.push_back(normalsV[i][j][2]); // Vector Normal
		// COLOR
		if (palcolor) {	//	Colorejar segons al�ada sobre la paleta de colors colorR,colorG,colorB
						// <CAL FER: C�LCUL index>;
						colors.push_back(paletaR[index]);	colors.push_back(paletaG[index]);		colors.push_back(paletaB[index]); colors.push_back(1.0);// Vector Colors
		}
		else {	//	Colorejar segons color de l'objecte (FET)
				colors.push_back(cColor[0]);	 colors.push_back(cColor[1]);		colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
		}
		// TEXTURA
		if (textur) {	// CAL FER: <C�LCUL (ctx,cty)>
						textures.push_back(ctx); textures.push_back(cty);
					}
		else textures.push_back(1.0f); textures.push_back(0.0f);				// Vector Textures
		// VERTEX
		vertices.push_back(i+step);	vertices.push_back(j);	vertices.push_back(zz[i+step][j]);	// Vector Vertices

		// ---------- V�RTEX 3
		// VECTOR NORMAL
		//normals.push_back();		normals.push_back();	normals.push_back();	// Vector Normal
		// COLOR
		if (palcolor) {	//	Colorejar segons al�ada sobre la paleta de colors colorR,colorG,colorB
						// CAL FER: <C�LCUL index>;
						colors.push_back(paletaR[index]);	colors.push_back(paletaG[index]);		colors.push_back(paletaB[index]); colors.push_back(1.0);// Vector Colors
		}
		else {	//	Colorejar segons color de l'objecte (FET)
				colors.push_back(cColor[0]);	 colors.push_back(cColor[1]);		colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
			}
		// TEXTURA
		if (textur) {	// CAL FER: <C�LCUL (ctx,cty)>
						textures.push_back(ctx); textures.push_back(cty);
				}
		else {	textures.push_back(1.0f); textures.push_back(1.0f);				// Vector Textures
			}
		// VERTEX
		vertices.push_back(i+step);	vertices.push_back(j+step);	vertices.push_back(zz[i+step][j+step]);	// Vector Vertices

// ------------------------------- Segon Triangle ------------------------------------------
		// -------- V�RTEX 1
		// VECTOR NORMAL
		normals.push_back(normalsV[i][j][0]);		normals.push_back(normalsV[i][j][1]);	normals.push_back(normalsV[i][j][2]); // Vector Normal
		// COLOR
		if (palcolor) {	//	Colorejar segons al�ada sobre la paleta de colors colorR,colorG,colorB
						// CAL FER: <C�LCUL index>;
						colors.push_back(paletaR[index]);	colors.push_back(paletaG[index]);		colors.push_back(paletaB[index]); colors.push_back(1.0);// Vector Colors
		}
		else {	//	Colorejar segons color de l'objecte
				colors.push_back(cColor[0]);	 colors.push_back(cColor[1]);		colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
		}
		// TEXTURA
		if (textur) {	// CAL FER: <C�LCUL (ctx,cty)>
						textures.push_back(ctx); textures.push_back(cty);			// Vector Textures
		}
		else {	textures.push_back(0.0f); textures.push_back(0.0f);				// Vector Textures
			}
		// VERTEX
		vertices.push_back(i);	vertices.push_back(j);	vertices.push_back(zz[i][j]);	// Vector Vertices

		// -------- V�RTEX 3
			// VECTOR NORMAL
		normals.push_back(normalsV[i][j][0]);		normals.push_back(normalsV[i][j][1]);	normals.push_back(normalsV[i][j][2]); // Vector Normal
		// COLOR
		if (palcolor) {	//	Colorejar segons al�ada sobre la paleta de colors colorR,colorG,colorB
						// CAL FER: <C�LCUL index>;
						colors.push_back(paletaR[index]);	colors.push_back(paletaG[index]);		colors.push_back(paletaB[index]); colors.push_back(1.0);// Vector Colors
		}
		else {	//	Colorejar segons color de l'objecte (FET)
				colors.push_back(cColor[0]);	 colors.push_back(cColor[1]);		colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
		}
		//	Assignaci� TEXTURA
		if (textur) {	// CAL FER: <C�LCUL (ctx,cty)>
						textures.push_back(ctx); textures.push_back(cty);			// Vector Textures
		}
		else {	textures.push_back(1.0f); textures.push_back(1.0f);				// Vector Textures
			}
		// VERTEX
		vertices.push_back(i+step);	vertices.push_back(j+step);	vertices.push_back(zz[i+step][j+step]);	// Vector Vertices


		// -------- V�RTEX 4
		// VECTOR NORMAL
		normals.push_back(normalsV[i][j][0]);		normals.push_back(normalsV[i][j][1]);	normals.push_back(normalsV[i][j][2]); // Vector Normal
		// COLOR
		if (palcolor) {	//	Colorejar segons al�ada sobre la paleta de colors colorR,colorG,colorB
						// CAL FER: <C�LCUL index>;
						colors.push_back(paletaR[index]);	colors.push_back(paletaG[index]);		colors.push_back(paletaB[index]); colors.push_back(1.0);// Vector Colors
		}
		else {	//	Colorejar segons color de l'objecte (FET)
				colors.push_back(cColor[0]);	 colors.push_back(cColor[1]);		colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
		}
		// TEXTURA
		if (textur) {	// CAL FER: <C�LCUL (ctx,cty)>
						textures.push_back(ctx); textures.push_back(cty);			// Vector Textures
		}
		else {	textures.push_back(0.0f); textures.push_back(1.0f);				// Vector Textures
			}
		// VERTEX
		vertices.push_back(i);	vertices.push_back(j+step);	vertices.push_back(zz[i][j+step]);	// Vector Vertices
	}

	else
// PER A IL.LUMINACI� PLANA o FILFERROS o PUNTS CAL DESENVOLUPAR EL SEG�ENT CODI:
	{
		// CAL FER: Definir els "for" per a recorrer la matriu amb indexs (i,j)

// ------------------------------- Primer Triangle ------------------------------------------
		// --------- V�RTEX 1
		// VECTOR NORMAL
		normals.push_back(normalsV[i][j][0]);		normals.push_back(normalsV[i][j][1]);	normals.push_back(normalsV[i][j][2]); // Vector Normal
		// COLOR
		if (palcolor) {	//	Colorejar segons al�ada sobre la paleta de colors colorR,colorG,colorB
						// CAL FER: <C�LCUL index>;
						colors.push_back(paletaR[index]);	colors.push_back(paletaG[index]);		colors.push_back(paletaB[index]); colors.push_back(1.0);// Vector Colors
					}
			else {	//	Colorejar segons color de l'objecte (FET)
					colors.push_back(cColor[0]);	 colors.push_back(cColor[1]);		colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
				}
		//	TEXTURA
		if (textur) {	// CAL FER: <C�LCUL (ctx,cty)>
						textures.push_back(ctx); textures.push_back(cty);			// Vector Textures
					}
			else {	textures.push_back(0.0f); textures.push_back(0.0f);				// Vector Textures
				}
		// VERTEX
		//vertices.push_back();	vertices.push_back();	vertices.push_back();	// Vector Vertices

		// --------- V�RTEX 2
		// VECTOR NORMAL
		normals.push_back(normalsV[i][j][0]);		normals.push_back(normalsV[i][j][1]);	normals.push_back(normalsV[i][j][2]); // Vector Normal
		// COLOR
		if (palcolor) {	//	Colorejar segons al�ada sobre la paleta de colors colorR,colorG,colorB
						// <CAL FER: C�LCUL index>;
						colors.push_back(paletaR[index]);	colors.push_back(paletaG[index]);		colors.push_back(paletaB[index]); colors.push_back(1.0);// Vector Colors
						}
			else {	//	Colorejar segons color de l'objecte (FET)
					colors.push_back(cColor[0]);	 colors.push_back(cColor[1]);		colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
				}
		//	TEXTURA
		if (textur) {	// CAL FER: <C�LCUL (ctx,cty)>
						textures.push_back(ctx); textures.push_back(cty);			// Vector Textures
					}
			else {	textures.push_back(1.0f); textures.push_back(0.0f);				// Vector Textures
				}
		// VERTEX
		vertices.push_back(i);	vertices.push_back(j);	vertices.push_back(zz[i][j]);	// Vector Vertices

		// --------- V�RTEX 3
		// VECTOR NORMAL
		//normals.push_back();		normals.push_back();	normals.push_back();	// Vector Normal
		if (palcolor) {	//	Colorejar segons al�ada sobre la paleta de colors colorR,colorG,colorB
						// CAL FER: <C�LCUL index>;
						colors.push_back(paletaR[index]);	colors.push_back(paletaG[index]);		colors.push_back(paletaB[index]); colors.push_back(1.0);// Vector Colors
					}
			else {	//	Colorejar segons color de l'objecte (FET)
					colors.push_back(cColor[0]);	 colors.push_back(cColor[1]);		colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
				}
		// TEXTURA
		if (textur) {	// CAL FER: <C�LCUL (ctx,cty)>
						textures.push_back(ctx); textures.push_back(cty);			// Vector Textures
					}
			else {	textures.push_back(1.0f); textures.push_back(1.0f);				// Vector Textures
				}
		// VERTEX
		vertices.push_back(i);	vertices.push_back(j);	vertices.push_back(zz[i][j]);	// Vector Vertices

// ------------------------------- Segon Triangle ------------------------------------------
		// -------- V�RTEX 1
		// VECTOR NORMAL
		normals.push_back(normalsV[i][j][0]);		normals.push_back(normalsV[i][j][1]);	normals.push_back(normalsV[i][j][2]); // Vector Normal
		// COLOR
		if (palcolor) {	//	Colorejar segons al�ada sobre la paleta de colors colorR,colorG,colorB
						// CAL FER: <C�LCUL index>;
						colors.push_back(paletaR[index]);	colors.push_back(paletaG[index]);		colors.push_back(paletaB[index]);	colors.push_back(1.0);// Vector Colors
						}
			else {	//	Colorejar segons color de l'objecte
					colors.push_back(cColor[0]);	 colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]);// Vector Colors
					}
		//	TEXTURA
		if (textur) {	// CAL FER: <C�LCUL (ctx,cty)>
						textures.push_back(ctx); textures.push_back(cty);			// Vector Textures
					}
			else {	textures.push_back(0.0f); textures.push_back(0.0f);				// Vector Textures
				}
		// VERTEX
		//vertices.push_back();	vertices.push_back();	vertices.push_back();	// Vector Vertices

		// -------- V�RTEX 3
		// VECTOR NORMAL
		//normals.push_back();		normals.push_back();	normals.push_back();	// Vector Normal
		// COLOR
		if (palcolor) {	//	Colorejar segons al�ada sobre la paleta de colors colorR,colorG,colorB
						// CAL FER: <C�LCUL index>;
						colors.push_back(paletaR[index]);	colors.push_back(paletaG[index]);		colors.push_back(paletaB[index]); colors.push_back(1.0);// Vector Colors
						}
			else {	//	Colorejar segons color de l'objecte (FET)
					colors.push_back(cColor[0]);	 colors.push_back(cColor[1]);		colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
				}
		//	TEXTURA
		if (textur) {	// CAL FER: <C�LCUL (ctx,cty)>
						textures.push_back(ctx); textures.push_back(cty);			// Vector Textures
					}
			else {	textures.push_back(1.0f); textures.push_back(1.0f);				// Vector Textures
				}
		// VERTEX
		vertices.push_back(i);	vertices.push_back(j);	vertices.push_back(zz[i][j]);	// Vector Vertices

		// -------- V�RTEX 4
		// VECTOR NORMAL
		normals.push_back(normalsV[i][j][0]);		normals.push_back(normalsV[i][j][1]);	normals.push_back(normalsV[i][j][2]); // Vector Normal
		// COLOR
		if (palcolor) {	//	Colorejar segons al�ada sobre la paleta de colors colorR,colorG,colorB
						// CAL FER: <C�LCUL index>;
						colors.push_back(paletaR[index]);	colors.push_back(paletaG[index]);		colors.push_back(paletaB[index]); colors.push_back(1.0);// Vector Colors
						}
			else {	//	Colorejar segons color de l'objecte (FET)
					colors.push_back(cColor[0]);	 colors.push_back(cColor[1]);		colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
				}
		//	TEXTURA
		if (textur) {	// CAL FER: <C�LCUL (ctx,cty)>
						textures.push_back(ctx); textures.push_back(cty);			// Vector Textures
					}
			else {	textures.push_back(0.0f); textures.push_back(1.0f);				// Vector Textures
			}
		// VERTEX
		vertices.push_back(i);	vertices.push_back(j);	vertices.push_back(zz[i][j]);	// Vector Vertices
	}

// ----------------------- VAO
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

	FractalVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);

	return FractalVAO;
}


// loadfractEBO: TRIANGULACIO DEL TERRENY per VAO(Vertex Array Objects) AMB EBO (Element Buffer Objects)
// Par�metres: - palcolor: Booleana que indica si la coloraci� per paleta �s activa [TRUE] o el coklor del fractal �s el monocolor segons el color de l'objecte [FALSE].
//             - paleta: Tipus de paleta si palcolor �s TRUE. Pot agafar els valors de: MEDITERRANI, TUNDRA o GLACIAR.
//             - iluminaci�: Tipus d'ilumina'
//             - ref_mat: Booleana d'activaci� de la reflectivitat de material, una forma de donar color. Usualment, false.
//			   - sw_mat: Booleana d'activaci� dels diferents materials (emissi�[0], ambient[1], difusa[2], especular[3]),
//					[4]: booleana per a definir color segons el material (TRUE) o segons vector color del VAO -vector colors- (FALSE).
//					Par�metre a passar a funci� SeleccionaColorMaterial() del m�dul material.cpp
//			   - textur: Booleana d'activaci� per a texturar el fractal [TRUE] o no [FALSE]
//			   - step: Pas de recorregut per a formar els triangles. M�xim: el valor calculat adel fitxer fractal. Minim: 1.
//			   - vaoID: Identificador de VAO associat al fractal creat. S'utilitzar� posteriorment per a dibuixar el fractal (VAO)
//			   - nvertexs: Nombre de v�rtexs generat per a dibuixar el fractal. En funci� del pas de recoregut definit en el par�metre step.
CVAO loadfractEBO(bool palcolor, char paleta, char iluminacio, bool sw_mat[5], bool textur, int step)
{
	int i, j;
	float ctx, cty;
	CColor col_Vertex;								// Color del v�rtex del fractal.
	float paletaR[MAX_PALETA_FRACTAL], paletaG[MAX_PALETA_FRACTAL], paletaB[MAX_PALETA_FRACTAL];	// Paleta de colors

	i = 0;		j = 0;		
	ctx = 0.0f;	cty = 0.0f;
	col_Vertex.r = 0.0;		col_Vertex.g = 0.0;		col_Vertex.b = 0.0;
	for (i = 0; i < MAX_PALETA_FRACTAL; i++) { paletaR[i] = 0.0f; paletaG[i] = 0.0f;	paletaB[i] = 0.0f; }

	// VAO
	CVAO FractalVAO;
	FractalVAO.vaoId = 0;	FractalVAO.vboId = 0;	FractalVAO.nVertexs = 0;	// Valors inicialitzats per a VAO sense EBO.
	FractalVAO.eboId = 0;	FractalVAO.nIndices = 0;								// Valors inicialitzats per a VAO amb EBO.

	std::vector <double> vertices, normals, colors, textures;	// Definici� vectors din�mics per a vertexs, normals i textures 
	std::vector <uint> indices;
	vertices.clear();			normals.clear();		colors.clear();		textures.clear();	// Reinicialitzar vectors
	indices.clear();

	GLint index = 0;		// Apuntador a v�rtexs per a indices.

// Obtenir color actual definit en OpenGL amb glColor();
	GLfloat cColor[4];
	glGetFloatv(GL_CURRENT_COLOR, cColor);

	switch (paleta)
	{
	case MEDITERRANI:
		// CAL FER: Configurar paleta mediterrani

		break;
	case GLACIAR:
		// CAL FER: Configurar paleta glaciar

		break;

	case TUNDRA:
		// CAL FER: Configurar paleta tundra

		break;

	default:
		// CAL FER: Configurar paleta color blanc
		break;
	}

// CAL FER: C�lcul del valor de traslaci� per a centrar el fractal aplicat a tots els v�rtexs del fractal.
	const float centrat = FMAX / 2.f;

// 2. CAL FER: CREAR ELS V�RTEXS (x,y,z,1) DELS TRIANGLES SEGONS EL PAS (step),
//				DEFINIR ELS VECTORS NORMALS (VNx,VNy,VNz) DE CADA V�RTEX EN FUNCI� DE
//				LA ILUMINACI� (valiable iluminacio), EL COLOR (r,g,b,a) DE CADA V�RTEX SEGONS LA PALETA I LES COORDENADES TEXTURA (ctx,cty).

	if ((iluminacio == GOURAUD) || (iluminacio == PHONG))
	{
		// PER A IL.LUMINACI� GOURAUD CAL DESENVOLUPAR EL SEG�ENT CODI:

		// CAL FER: Definir els "for" per a recorrer la matriu amb indexs (i,j)

// ------------------------------- Definici� dels v�rtexs del Quadrat ------------------------------------------
		// -------- V�RTEX 1
		// VECTOR NORMAL
		normals.push_back(normalsV[i][j][0]);		normals.push_back(normalsV[i][j][1]);	normals.push_back(normalsV[i][j][2]); // Vector Normal
		// COLOR
		if (palcolor) {	//	Colorejar segons al�ada sobre la paleta de colors colorR,colorG,colorB
			// CAL FER: <C�LCUL index>;
			colors.push_back(paletaR[index]);	colors.push_back(paletaG[index]);		colors.push_back(paletaB[index]); colors.push_back(1.0);// Vector Colors
		}
		else {	//	Colorejar segons color de l'objecte (FET)
			colors.push_back(cColor[0]);	 colors.push_back(cColor[1]);		colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
		}
		//	Assignaci� TEXTURA
		if (textur) {	// CAL FER: <C�LCUL (ctx,cty)>
			textures.push_back(ctx); textures.push_back(cty);			// Vector Textures
		}
		else {
			textures.push_back(0.0f); textures.push_back(0.0f);				// Vector Textures
		}
		// VERTEX
		vertices.push_back(i);	vertices.push_back(j);	vertices.push_back(zz[i][j]);	// Vector Vertices

		// -------- V�RTEX 2
		// VECTOR NORMAL
		normals.push_back(normalsV[i][j][0]);		normals.push_back(normalsV[i][j][1]);	normals.push_back(normalsV[i][j][2]); // Vector Normal
		// COLOR
		if (palcolor) {	//	Colorejar segons al�ada sobre la paleta de colors colorR,colorG,colorB
			// <CAL FER: C�LCUL index>;
			colors.push_back(paletaR[index]);	colors.push_back(paletaG[index]);		colors.push_back(paletaB[index]); colors.push_back(1.0);// Vector Colors
		}
		else {	//	Colorejar segons color de l'objecte (FET)
			colors.push_back(cColor[0]);	 colors.push_back(cColor[1]);		colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
		}
		// TEXTURA
		if (textur) {	// CAL FER: <C�LCUL (ctx,cty)>
			textures.push_back(ctx); textures.push_back(cty);
		}
		else textures.push_back(1.0f); textures.push_back(0.0f);				// Vector Textures
		// VERTEX
		vertices.push_back(i);	vertices.push_back(j);	vertices.push_back(zz[i][j]);	// Vector Vertices

		// ---------- V�RTEX 3
		// VECTOR NORMAL
		normals.push_back(normalsV[i][j][0]);		normals.push_back(normalsV[i][j][1]);	normals.push_back(normalsV[i][j][2]); // Vector Normal
		// COLOR
		if (palcolor) {	//	Colorejar segons al�ada sobre la paleta de colors colorR,colorG,colorB
			// CAL FER: <C�LCUL index>;
			colors.push_back(paletaR[index]);	colors.push_back(paletaG[index]);		colors.push_back(paletaB[index]); colors.push_back(1.0);// Vector Colors
		}
		else {	//	Colorejar segons color de l'objecte (FET)
			colors.push_back(cColor[0]);	 colors.push_back(cColor[1]);		colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
		}
		// TEXTURA
		if (textur) {	// CAL FER: <C�LCUL (ctx,cty)>
			textures.push_back(ctx); textures.push_back(cty);
		}
		else {
			textures.push_back(1.0f); textures.push_back(1.0f);				// Vector Textures
		}
		// VERTEX
		vertices.push_back(i);	vertices.push_back(j);	vertices.push_back(zz[i][j]);	// Vector Vertices

		// -------- V�RTEX 4
		// VECTOR NORMAL
		normals.push_back(normalsV[i][j][0]);		normals.push_back(normalsV[i][j][1]);	normals.push_back(normalsV[i][j][2]); // Vector Normal
		// COLOR
		if (palcolor) {	//	Colorejar segons al�ada sobre la paleta de colors colorR,colorG,colorB
			// CAL FER: <C�LCUL index>;
			colors.push_back(paletaR[index]);	colors.push_back(paletaG[index]);		colors.push_back(paletaB[index]); colors.push_back(1.0);// Vector Colors
		}
		else {	//	Colorejar segons color de l'objecte (FET)
			colors.push_back(cColor[0]);	 colors.push_back(cColor[1]);		colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
		}
		// TEXTURA
		if (textur) {	// CAL FER: <C�LCUL (ctx,cty)>
			textures.push_back(ctx); textures.push_back(cty);			// Vector Textures
		}
		else {
			textures.push_back(0.0f); textures.push_back(1.0f);				// Vector Textures
		}
		// VERTEX
		vertices.push_back(i);	vertices.push_back(j);	vertices.push_back(zz[i][j]);	// Vector Vertices

// CAL FER: Definir els �ndexs per al primer i segon triangle
// ------------------------------- Primer Triangle ------------------------------------------
		// Vector indices Primer Triangle
		indices.push_back(index);		indices.push_back(index);		indices.push_back(index);	// v1-v2-v3

// ------------------------------- Segon Triangle ------------------------------------------
		indices.push_back(index);		indices.push_back(index);		indices.push_back(index);	// v1-v3-v4

// CAL FER: Incrementar index al seg�ent quadrat
		// index = index + ;

	}
	else
	{	// PER A IL.LUMINACI� PLANA o FILFERROS o PUNTS CAL DESENVOLUPAR EL SEG�ENT CODI:

		// CAL FER: Definir els "for" per a recorrer la matriu amb indexs (i,j)

// ------------------------------- Definici� dels v�rtexs del Quadrat ------------------------------------------
		// --------- V�RTEX 1
		// VECTOR NORMAL
		normals.push_back(normalsV[i][j][0]);		normals.push_back(normalsV[i][j][1]);	normals.push_back(normalsV[i][j][2]); // Vector Normal
		// COLOR
		if (palcolor) {	//	Colorejar segons al�ada sobre la paleta de colors colorR,colorG,colorB
			// CAL FER: <C�LCUL index>;
			colors.push_back(paletaR[index]);	colors.push_back(paletaG[index]);		colors.push_back(paletaB[index]); colors.push_back(1.0);// Vector Colors
		}
		else {	//	Colorejar segons color de l'objecte (FET)
			colors.push_back(cColor[0]);	 colors.push_back(cColor[1]);		colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
		}
		//	TEXTURA
		if (textur) {	// CAL FER: <C�LCUL (ctx,cty)>
			textures.push_back(ctx); textures.push_back(cty);			// Vector Textures
		}
		else {
			textures.push_back(0.0f); textures.push_back(0.0f);				// Vector Textures
		}
		// VERTEX
		//vertices.push_back();	vertices.push_back();	vertices.push_back();	// Vector Vertices

		// --------- V�RTEX 2
		// VECTOR NORMAL
		normals.push_back(normalsV[i][j][0]);		normals.push_back(normalsV[i][j][1]);	normals.push_back(normalsV[i][j][2]); colors.push_back(1.0);// Vector Normal
		// COLOR
		if (palcolor) {	//	Colorejar segons al�ada sobre la paleta de colors colorR,colorG,colorB
			// <CAL FER: C�LCUL index>;
			colors.push_back(paletaR[index]);	colors.push_back(paletaG[index]);		colors.push_back(paletaB[index]); colors.push_back(1.0);// Vector Colors
		}
		else {	//	Colorejar segons color de l'objecte (FET)
			colors.push_back(cColor[0]);	 colors.push_back(cColor[1]);		colors.push_back(cColor[2]); // Vector Colors
		}
		//	TEXTURA
		if (textur) {	// CAL FER: <C�LCUL (ctx,cty)>
			textures.push_back(ctx); textures.push_back(cty);			// Vector Textures
		}
		else {
			textures.push_back(1.0f); textures.push_back(0.0f);				// Vector Textures
		}
		// VERTEX
		vertices.push_back(i);	vertices.push_back(j);	vertices.push_back(zz[i][j]);	// Vector Vertices

		// --------- V�RTEX 3
		// VECTOR NORMAL
		normals.push_back(normalsV[i][j][0]);		normals.push_back(normalsV[i][j][1]);	normals.push_back(normalsV[i][j][2]); // Vector Normal
		if (palcolor) {	//	Colorejar segons al�ada sobre la paleta de colors colorR,colorG,colorB
			// CAL FER: <C�LCUL index>;
			colors.push_back(paletaR[index]);	colors.push_back(paletaG[index]);		colors.push_back(paletaB[index]); colors.push_back(1.0);// Vector Colors
		}
		else {	//	Colorejar segons color de l'objecte (FET)
			colors.push_back(cColor[0]);	 colors.push_back(cColor[1]);		colors.push_back(cColor[2]); colors.push_back(1.0);// Vector Colors
		}
		// TEXTURA
		if (textur) {	// CAL FER: <C�LCUL (ctx,cty)>
			textures.push_back(ctx); textures.push_back(cty);			// Vector Textures
		}
		else {
			textures.push_back(1.0f); textures.push_back(1.0f);				// Vector Textures
		}
		// VERTEX
		vertices.push_back(i);	vertices.push_back(j);	vertices.push_back(zz[i][j]);	// Vector Vertices

		// -------- V�RTEX 4
		// VECTOR NORMAL
		normals.push_back(normalsV[i][j][0]);		normals.push_back(normalsV[i][j][1]);	normals.push_back(normalsV[i][j][2]); // Vector Normal
		// COLOR
		if (palcolor) {	//	Colorejar segons al�ada sobre la paleta de colors colorR,colorG,colorB
			// CAL FER: <C�LCUL index>;
			colors.push_back(paletaR[index]);	colors.push_back(paletaG[index]);		colors.push_back(paletaB[index]); colors.push_back(1.0);// Vector Colors
		}
		else {	//	Colorejar segons color de l'objecte (FET)
			colors.push_back(cColor[0]);	 colors.push_back(cColor[1]);		colors.push_back(cColor[2]); colors.push_back(1.0);// Vector Colors
		}
		//	TEXTURA
		if (textur) {	// CAL FER: <C�LCUL (ctx,cty)>
			textures.push_back(ctx); textures.push_back(cty);			// Vector Textures
		}
		else {
			textures.push_back(0.0f); textures.push_back(1.0f);				// Vector Textures
		}
		// VERTEX
		vertices.push_back(i);	vertices.push_back(j);	vertices.push_back(zz[i][j]);	// Vector Vertices

// CAL FER: Definir els �ndexs per al primer i segon triangle
// ------------------------------- Primer Triangle ------------------------------------------
				// Vector indices Primer Triangle
		indices.push_back(index);		indices.push_back(index);		indices.push_back(index);	// v1-v2-v3

// ------------------------------- Segon Triangle ------------------------------------------
		indices.push_back(index);		indices.push_back(index);		indices.push_back(index);	// v1-v3-v4

// CAL FER: Incrementar index al seg�ent quadrat
		// index = index + ;
	}

	// ----------------------- VAO
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

	FractalVAO = load_TRIANGLES_EBO(vertices, normals, colors, textures, indices);
	return FractalVAO;
}


// loadMar_FractalVAO(): Definici� com a VAO d'un rectangle semitransparent a l'al�ada Z=0 fent de mar en el fractal.
CVAO loadMar_FractalVAO()
{
	CVAO MarFVAO;
	MarFVAO.vaoId = 0;			MarFVAO.vboId = 0;		MarFVAO.nVertexs = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definici� vectors din�mics per a vertexs, normals i textures 
	vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors

// Definir color del Mar Fractal
	GLfloat cColor[4];
	cColor[0] = 0.2f;		cColor[1] = 0.75f;		cColor[2] = 0.9f;		cColor[3] = 0.5f;

//  Dibuix el mar
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//  Dibuix el mar
	//glRectf(-FMAX / 2, -FMAX / 2, FMAX / 2, FMAX / 2); // Definici� del rectangle

// TRIANGLE 1: (1,2,3)
	// V�rtex 1 (FMAX/2,-FMAX/2)
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
	normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
	textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
	vertices.push_back(FMAX / 2);		vertices.push_back(-FMAX / 2);	vertices.push_back(0.0);		// Vector Vertices
	// V�rtex 2 (-FMAX/2,-FMAX/2)
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
	normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
	textures.push_back(0.0);		textures.push_back(1.0);										// Vector Textures
	vertices.push_back(-FMAX / 2);	vertices.push_back(-FMAX / 2);	vertices.push_back(0.0);		// Vector Vertices
	// V�rtex 3 (-FMAX/2,FMAX/2)
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
	normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
	textures.push_back(1.0);		textures.push_back(1.0);										// Vector Textures
	vertices.push_back(-FMAX / 2);	vertices.push_back(FMAX / 2);	vertices.push_back(0.0);		// Vector Vertices

// TRIANGLE 2: (1,3,4)
	// V�rtex 1 (FMAX/2,-FMAX/2)
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
	normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
	textures.push_back(0.0);		textures.push_back(0.0);										// Vector Textures
	vertices.push_back(FMAX / 2);	vertices.push_back(-FMAX / 2);	vertices.push_back(0.0);	// Vector Vertices
	// V�rtex 3 (-FMAX/2,FMAX/2)
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
	normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
	textures.push_back(1.0);		textures.push_back(1.0);										// Vector Textures
	vertices.push_back(-FMAX / 2);	vertices.push_back(FMAX / 2);	vertices.push_back(0.0);		// Vector Vertices
	// V�rtex 4 (FMAX/2,FMAX/2)
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
	normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
	textures.push_back(1.0);		textures.push_back(0.0);										// Vector Textures
	vertices.push_back(FMAX / 2);	vertices.push_back(FMAX / 2);	vertices.push_back(0.0);		// Vector Vertices

// ----------------------- VAO
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

	MarFVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);

	return MarFVAO;
}