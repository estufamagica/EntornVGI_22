//******** PRACTICA VISUALITZACI� GR�FICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn b�sic VS2022 MULTIFINESTRA amb OpenGL 3.3+, interf�cie MFC, llibreries GLM i primitives GLUT en codi font
//******** Marc Vivet, Carme Juli�, D�bora Gil, Enric Mart�  (Octubre 2022)
// normals.h : interface de normals.cpp
//             Declaracions de les funcions de c�lcul de 
//			   vectors normals per la visualitzaci� dels fractals 


#ifndef NORMALS
#define NORMALS

// -------------- Entorn VGI: Refer�ncia a les quadr�cules zz i normals, definides
//                            a fractals.cpp.
extern float zz[FMAX+1][513];
extern float normalsC[FMAX+1][FMAX+1][3];
extern float normalsV[FMAX+1][FMAX+1][3];

// normal: C�lcul del producte vectorial normalitzat de dos vectors v1,v2.
//         Resultat a la variable n.
void normal(float v1[3], float v2[3], float n[3]);

/// normalsvertex: C�lcul de les normals unit�ries als VERTEXS
//				  a una triangulaci� de la matriu zz segons el pas 'step'
//				  tenint en compte les normals de les superf�cies ve�nes.
//                S'utilitza en la il.luminaci� SUAU del fractal
void normalsvertex(int step);

// normalcara:	C�lcul de les normals unit�ries a les CARES 
//				per una triangulaci� de la matriu zz segons el pas 'step'
//				tenint en compte les normals de les superf�cies ve�nes.
//              S'utilitza en la il.luminaci� PLANA del fractal
void normalscara(int step);

#endif
