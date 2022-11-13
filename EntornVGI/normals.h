//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn bàsic VS2022 MULTIFINESTRA amb OpenGL 3.3+, interfície MFC, llibreries GLM i primitives GLUT en codi font
//******** Marc Vivet, Carme Julià, Débora Gil, Enric Martí  (Octubre 2022)
// normals.h : interface de normals.cpp
//             Declaracions de les funcions de càlcul de 
//			   vectors normals per la visualització dels fractals 


#ifndef NORMALS
#define NORMALS

// -------------- Entorn VGI: Referència a les quadrícules zz i normals, definides
//                            a fractals.cpp.
extern float zz[FMAX+1][513];
extern float normalsC[FMAX+1][FMAX+1][3];
extern float normalsV[FMAX+1][FMAX+1][3];

// normal: Càlcul del producte vectorial normalitzat de dos vectors v1,v2.
//         Resultat a la variable n.
void normal(float v1[3], float v2[3], float n[3]);

/// normalsvertex: Càlcul de les normals unitàries als VERTEXS
//				  a una triangulació de la matriu zz segons el pas 'step'
//				  tenint en compte les normals de les superfícies veïnes.
//                S'utilitza en la il.luminació SUAU del fractal
void normalsvertex(int step);

// normalcara:	Càlcul de les normals unitàries a les CARES 
//				per una triangulació de la matriu zz segons el pas 'step'
//				tenint en compte les normals de les superfícies veïnes.
//              S'utilitza en la il.luminació PLANA del fractal
void normalscara(int step);

#endif
