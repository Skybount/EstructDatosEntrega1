#include <iostream>
#include "resource.h"
#include <Windows.h>
#include <string>
#include <fstream>
#include <CommCtrl.h>
#include <regex>

using namespace std;

struct usuario {
	usuario* ant;
	char clave[50];
	char contra[50];
	char ApllPat[50];
	char ApllMat[50];
	char nombres[50];
	usuario* sig;
};

usuario* auxUsuario, *inicioUsuario, *miUsuario = nullptr;
void agregarUsuario(usuario* nuevo);
void escribirUsuarioBin();
void leerUsuarioBin();

struct carnet {
	carnet* ant;
	string persona;
	string vacuna;
	string dosis;
	string fechaDosis;
	string lote;
	string centroVacuna;
	carnet* sig;
};

carnet* auxCarnet, *inicioCarnet = nullptr;

struct persona {
	persona* ant;
	char ApllPat[50];
	char ApllMat[50];
	char nombres[50];
	char curp[25];
	char rfc[20];
	char fechaNacimiento[15];
	char calle[30];
	char colonia[30];
	char ciudad[20];
	char estado[30];
	char estadoCivil[20];
	char telefono[11];
	char docIdentidad[256];
	char sexo[12];
	char gpoOcupacional[20];
	char perfilRiesgo[25];
	char foto[256];
	persona* sig;
};

persona* auxPersona, *inicioPersona = nullptr;
void agregarPersona(persona* nuevo);
void escribirPersonaBin();
void leerPersonaBin();

struct vacuna {
	vacuna* ant;
	char tipo[15];
	char marca[15];
	char numDosis[10];
	char claveVacuna[20];
	char descripcion[50];
	float precio = 0;
	vacuna* sig;
};

vacuna* auxVacuna, *inicioVacuna = nullptr;
void agregarVacuna(vacuna* nuevo);
void escribirVacunaBin();
void leerVacunasBin();

char zFile[MAX_PATH];
char yFile[MAX_PATH];

LRESULT CALLBACK procVInicio(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVMenu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVRegistrarPersona(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK procVRegistrarVacuna(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HINSTANCE hInstGlobal;
HWND hInstMenu;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
	leerUsuarioBin();
	//leerPersonaBin();
	leerVacunasBin();
	hInstGlobal = hInst;
	HWND hVInicio = CreateDialog(hInst, MAKEINTRESOURCE(101), NULL, procVInicio);

	ShowWindow(hVInicio, cmdshow);

	MSG Mensaje;
	ZeroMemory(&Mensaje, sizeof(Mensaje));

	while (GetMessage(&Mensaje, 0, 0, 0)) {
		TranslateMessage(&Mensaje);
		DispatchMessage(&Mensaje);
	}
	escribirUsuarioBin();
	escribirPersonaBin();
	escribirVacunaBin();
	return Mensaje.wParam;
}

LRESULT CALLBACK procVInicio(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_bIniciar: {
			char userClaveChar[50];
			char userContraChar[50];
			GetDlgItemText(hwnd, ID_InicioSesClave, userClaveChar, sizeof(userClaveChar));
			GetDlgItemText(hwnd, ID_InicioSesContra, userContraChar, sizeof(userContraChar));
			string userClave = userClaveChar;
			string userContra = userContraChar;

			auxUsuario = inicioUsuario;
			if (inicioUsuario == nullptr) {
				MessageBox(hwnd, "No hay usuario registrados.", "AVISO", MB_OK | MB_ICONERROR);
			}
			else {
				while (auxUsuario != nullptr && auxUsuario->clave != userClave) {
					auxUsuario = auxUsuario->sig;
				}
				if (auxUsuario == nullptr) {
					MessageBox(hwnd, "Usuario no encontrado", "AVISO", MB_OK | MB_ICONERROR);
				}
				else {
					if (auxUsuario->contra == userContra) {
						miUsuario = auxUsuario;
						EndDialog(hwnd, 0);
						DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_Menu), hwnd, procVMenu);
					}
					else {
						MessageBox(hwnd, "Contraseña incorrecta", "AVISO", MB_OK | MB_ICONERROR);
					}
				}
			}
		} break;
		case ID_bRegistrarse: {
			bool validClave = true, validContra1 = false, validContra2 = false;
			usuario* newUsuario = new usuario;
			GetDlgItemText(hwnd, ID_RegistrarClave, newUsuario->clave, sizeof(newUsuario->clave));
			GetDlgItemText(hwnd, ID_RegistrarContra, newUsuario->contra, sizeof(newUsuario->contra));
			GetDlgItemText(hwnd, ID_RegistrarApllPat, newUsuario->ApllPat, sizeof(newUsuario->ApllPat));
			GetDlgItemText(hwnd, ID_RegistrarApllMat, newUsuario->ApllMat, sizeof(newUsuario->ApllMat));
			GetDlgItemText(hwnd, ID_RegistrarNombre, newUsuario->nombres, sizeof(newUsuario->nombres));

			if (newUsuario->clave != "" && newUsuario->contra != "" && newUsuario->ApllPat != "" && newUsuario->ApllMat != "" && newUsuario->nombres != "") {
				auxUsuario = inicioUsuario;
				while (auxUsuario != nullptr) {
					if (strcmp(auxUsuario->clave, newUsuario->clave) == 0) {
						validClave = false;
						break;
					}
					else {
						validClave = true;
						auxUsuario = auxUsuario->sig;
					}
				}
				if (validClave == true) {
					agregarUsuario(newUsuario);
					delete (newUsuario);
					SetDlgItemText(hwnd, ID_RegistrarClave, "");
					SetDlgItemText(hwnd, ID_RegistrarContra, "");
					SetDlgItemText(hwnd, ID_RegistrarApllPat, "");
					SetDlgItemText(hwnd, ID_RegistrarApllPat, "");
					SetDlgItemText(hwnd, ID_RegistrarApllMat, "");
					SetDlgItemText(hwnd, ID_RegistrarNombre, "");
				}
				else {
					MessageBox(hwnd, "Esa clave de usuario ya esta siendo usada", "AVISO", MB_OK | MB_ICONERROR);
				}
			}
			else {
				MessageBox(hwnd, "No pueden haber campos vacios", "AVISO", MB_OK | MB_ICONERROR);
			}
		} break;
		}
	}break;
	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		if (opc == IDYES) {
			DestroyWindow(hwnd);
		}
	}break;
	case WM_DESTROY: {
		PostQuitMessage(0);
	}break;
	}
	return false;
}

LRESULT CALLBACK procVMenu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_Salir: {
			int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			if (opc == IDYES) {
				DestroyWindow(hwnd);
			}
		}break;
		case ID_PERSONAS_REGISTRAR: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_RegisPersona), hwnd, procVRegistrarPersona);
		}break;
		case ID_VACUNAS_REGISTRAR: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(ID_RegistrarVacuna), hwnd, procVRegistrarVacuna);
		}break;
		}
	}break;
	case WM_DESTROY: {
		PostQuitMessage(0);
	}break;
	}
	return false;
}

LRESULT CALLBACK procVRegistrarPersona(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	SYSTEMTIME RegisFecha;
	switch (msg) {
	case WM_INITDIALOG: {
		char sexo[2][10] = { "Masculino","Femenino" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, ID_RegisPersonaSexo, CB_INSERTSTRING, (WPARAM)i, (LPARAM)sexo[i]);
		}
		char EstadoCivil[6][15] = { "Casado","Conviviente","Divorciado","Separado","Viudo","Soltero" };
		char PerfilRiesgo[6][22] = { "Ninguno","Embarazada","Menor de 5 años","Enfermedad cronica","Obesidad o sobrepeso","Mayor de 60 años" };
		char GrupoOcupacional[6][16] = { "Doctor","Ingeniero", "Maestro", "Piloto", "Secretari@","Artista" };
		for (int j = 0; j < 6; j++) {
			SendDlgItemMessage(hwnd, ID_RegisPersonaEstCivil, CB_INSERTSTRING, (WPARAM)j, (LPARAM)EstadoCivil[j]);
			SendDlgItemMessage(hwnd, ID_RegisPersonaPrflRiesgo, CB_INSERTSTRING, (WPARAM)j, (WPARAM)PerfilRiesgo[j]);
			SendDlgItemMessage(hwnd, ID_RegisPersonaGpoOcpcnl, CB_INSERTSTRING, (WPARAM)j, (LPARAM)GrupoOcupacional[j]);
		}
		SendMessage(GetDlgItem(hwnd, ID_RegisPersonaSexo), CB_SETCURSEL, (WPARAM)0, 0);
		SendMessage(GetDlgItem(hwnd, ID_RegisPersonaGpoOcpcnl), CB_SETCURSEL, (WPARAM)0, 0);
		SendMessage(GetDlgItem(hwnd, ID_RegisPersonaEstCivil), CB_SETCURSEL, (WPARAM)0, 0);
		SendMessage(GetDlgItem(hwnd, ID_RegisPersonaPrflRiesgo), CB_SETCURSEL, (WPARAM)0, 0);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_bRegisPersonaFoto: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP bmp;

				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				SendDlgItemMessage(hwnd, ID_RegisPersonaFoto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			}
			else {
				MessageBox(hwnd, "No seleccionó foto", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;
		case ID_bRegisPersonaDocumento: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = yFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(yFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP bmp;

				bmp = (HBITMAP)LoadImage(NULL, yFile, IMAGE_BITMAP, 175, 100, LR_LOADFROMFILE);
				SendDlgItemMessage(hwnd, ID_RegisPersonaDocumento, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			}
			else {
				MessageBox(hwnd, "No seleccionó foto", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;
		case ID_bRegistrarPersona: {
			bool validCURP = false, validRFC = false, validCURP2 = true, validRFC2 = true;
			persona* newPersona = new persona;
			strcpy_s(newPersona->foto, zFile);
			strcpy_s(newPersona->docIdentidad, yFile);
			GetDlgItemText(hwnd, ID_RegisPersonaApllPat, newPersona->ApllPat, sizeof(newPersona->ApllPat));
			GetDlgItemText(hwnd, ID_RegisPersonaApllMat, newPersona->ApllMat, sizeof(newPersona->ApllMat));
			GetDlgItemText(hwnd, ID_RegisPersonaNombres, newPersona->nombres, sizeof(newPersona->nombres));
			GetDlgItemText(hwnd, ID_RegisPersonaCurp, newPersona->curp, sizeof(newPersona->curp));
			GetDlgItemText(hwnd, ID_RegisPersonaRFC, newPersona->rfc, sizeof(newPersona->rfc));
			GetDlgItemText(hwnd, ID_RegisPersonaCalle, newPersona->calle, sizeof(newPersona->calle));
			GetDlgItemText(hwnd, ID_RegisPersonaColonia, newPersona->colonia, sizeof(newPersona->colonia));
			GetDlgItemText(hwnd, ID_RegisPersonaCiudad, newPersona->ciudad, sizeof(newPersona->ciudad));
			GetDlgItemText(hwnd, ID_RegisPersonaestado, newPersona->estado, sizeof(newPersona->estado));
			GetDlgItemText(hwnd, ID_RegisPersonaTelefono, newPersona->telefono, sizeof(newPersona->telefono));
			GetDlgItemText(hwnd, ID_RegisPersonaSexo, newPersona->sexo, sizeof(newPersona->sexo));
			GetDlgItemText(hwnd, ID_RegisPersonaGpoOcpcnl, newPersona->gpoOcupacional, sizeof(newPersona->gpoOcupacional));
			GetDlgItemText(hwnd, ID_RegisPersonaPrflRiesgo, newPersona->perfilRiesgo, sizeof(newPersona->perfilRiesgo));
			GetDlgItemText(hwnd, ID_RegisPersonaEstCivil, newPersona->estadoCivil, sizeof(newPersona->estadoCivil));
			SendDlgItemMessage(hwnd, ID_RegisPersonaFechaNacimiento, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (RegisFecha));
			char fechaCompleta2[11] = { 0 };
			char charFecha[5] = { 0 };
			sprintf_s(charFecha, "%d", RegisFecha.wDay);
			strcat_s(fechaCompleta2, charFecha);
			strcat_s(fechaCompleta2, "/");
			sprintf_s(charFecha, "%d", RegisFecha.wMonth);
			strcat_s(fechaCompleta2, charFecha);
			strcat_s(fechaCompleta2, "/");
			sprintf_s(charFecha, "%d", RegisFecha.wYear);
			strcat_s(fechaCompleta2, charFecha);

			if (strlen(newPersona->curp) != 18) {
				validCURP = false;
			}
			else {
				validCURP = true;
			}

			if (strlen(newPersona->rfc) != 13) {
				validRFC = false;
			}
			else {
				validRFC = true;
			}
			if (validCURP == true) {
				if (validRFC == true) {
					if (newPersona->ApllPat != "" && newPersona->ApllMat != "" && newPersona->nombres != "" && newPersona->curp != "" && newPersona->rfc != "" && newPersona->telefono != "" && newPersona->docIdentidad != "" && newPersona->foto != "") {
						auxPersona = inicioPersona;
						while (auxPersona != nullptr) {
							if (strcmp(auxPersona->curp, newPersona->curp) == 0) {
								validCURP2 = false;
								break;
							}
							else {
								validCURP2 = true;
								auxPersona = auxPersona->sig;
							}
						}
						
						auxPersona = inicioPersona;
						while (auxPersona != nullptr) {
							if (strcmp(auxPersona->rfc, newPersona->rfc) == 0) {
								validRFC2 = false;
								break;
							}
							else {
								validRFC2 = true;
								auxPersona = auxPersona->sig;
							}
						}

						if (validCURP2 == true) {
							if (validRFC2 == true) {
								agregarPersona(newPersona);
								delete(newPersona);
								SetDlgItemText(hwnd, ID_RegisPersonaApllPat, "");
								SetDlgItemText(hwnd, ID_RegisPersonaApllMat, "");
								SetDlgItemText(hwnd, ID_RegisPersonaNombres, "");
								SetDlgItemText(hwnd, ID_RegisPersonaCurp, "");
								SetDlgItemText(hwnd, ID_RegisPersonaRFC, "");
								SetDlgItemText(hwnd, ID_RegisPersonaTelefono, "");
								SetDlgItemText(hwnd, ID_RegisPersonaDireccion, "");
								SetDlgItemText(hwnd, ID_RegisPersonaDocumento, "");
								SetDlgItemText(hwnd, ID_RegisPersonaFoto, "");
							}
							else {
								MessageBox(hwnd, "Ese RFC ya esta en uso", "AVISO", MB_OK | MB_ICONERROR);
							}
						}
						else {
							MessageBox(hwnd, "Ese CURP ya esta en uso", "AVISO", MB_OK | MB_ICONERROR);
						}
					}
					else {
						MessageBox(hwnd, "No pueden haber campos vacios", "AVISO", MB_OK | MB_ICONERROR);
					}
				}
				else {
					MessageBox(hwnd, "La RFC tiene que ser de 13 caracteres", "AVISO", MB_OK | MB_ICONERROR);
				}	
			}
			else {
				MessageBox(hwnd, "La CURP tiene que ser de 18 caracteres", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;
		case ID_bRegisPersonaSalir: {
			EndDialog(hwnd, 0);
		}break;
		}
	}break;
	}
	return false;
}

LRESULT CALLBACK procVRegistrarVacuna(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	char RegisPrecio[10] = "asd";
	bool ValidPrecio = false, ValidClave = true;
	switch (msg) {
	case WM_INITDIALOG: {
		char Tipo[3][30] = { "ARN","Vectorviral","Subunidades proteicas" };
		char Marca[4][15] = { "Pfizer","AstraZenaca","Casino","Moderna" };
		for (int i = 0; i < 3; i++) {
			SendDlgItemMessage(hwnd, ID_RegisVacunaTipo, CB_INSERTSTRING, (WPARAM)i, (LPARAM)Tipo[i]);
		}
		for (int j = 0; j < 4; j++) {
			SendDlgItemMessage(hwnd, ID_RegisVacunaMarca, CB_INSERTSTRING, (WPARAM)j, (LPARAM)Marca[j]);
		}
		SendMessage(GetDlgItem(hwnd, ID_RegisVacunaTipo), CB_SETCURSEL, (WPARAM)0, 0);
		SendMessage(GetDlgItem(hwnd, ID_RegisVacunaMarca), CB_SETCURSEL, (WPARAM)0, 0);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_bRegistrarVacuna: {
			vacuna* newVacuna = new vacuna;
			GetDlgItemText(hwnd, ID_RegisVacunaTipo, newVacuna->tipo, sizeof(newVacuna->tipo));
			GetDlgItemText(hwnd, ID_RegisVacunaMarca, newVacuna->marca, sizeof(newVacuna->marca));
			GetDlgItemText(hwnd, ID_RegisVacunaNo, newVacuna->numDosis, sizeof(newVacuna->numDosis));
			GetDlgItemText(hwnd, ID_RegisVacunaClave, newVacuna->claveVacuna, sizeof(newVacuna->claveVacuna));
			GetDlgItemText(hwnd, ID_RegisVacunaDesc, newVacuna->descripcion, sizeof(newVacuna->descripcion));
			GetDlgItemText(hwnd, ID_RegisVacunaPrecio, RegisPrecio, sizeof(RegisPrecio));
			newVacuna->precio = atof(RegisPrecio);

			if (newVacuna->precio > 0) {
				ValidPrecio = true;
			}
			else {
				ValidPrecio = false;
			}

			auxVacuna = inicioVacuna;
			while (auxVacuna != nullptr) {
				if (auxVacuna->claveVacuna == newVacuna->claveVacuna) {
					ValidClave = false;
					break;
				}
				else {
					ValidClave = true;
					auxVacuna = auxVacuna->sig;
				}
			}

			if (newVacuna->numDosis != "" && newVacuna->claveVacuna != "" && newVacuna->descripcion != "") {
				if (ValidPrecio == true) {
					if (ValidClave == true) {
						agregarVacuna(newVacuna);
						delete(newVacuna);
						SetDlgItemText(hwnd, ID_RegisVacunaNo, "");
						SetDlgItemText(hwnd, ID_RegisVacunaClave, "");
						SetDlgItemText(hwnd, ID_RegisVacunaDesc, "");
						SetDlgItemText(hwnd, ID_RegisVacunaPrecio, "");
					}
					else {
						MessageBox(hwnd, "Esa clave ya esta en uso", "AVISO", MB_OK | MB_ICONERROR);
					}
				}
				else {
					MessageBox(hwnd, "El precio tiene que ser mayor que 0", "AVISO", MB_OK | MB_ICONERROR);
				}
			}
			else {
				MessageBox(hwnd, "No pueden haber campos vacios", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;
		case ID_bRegisVacunaSalir: {
			EndDialog(hwnd, 0);
		}break;
		}
	}break;
	}

	return false;
}

void agregarUsuario(usuario* nuevo) {
	if (inicioUsuario == nullptr) {
		inicioUsuario = new usuario;
		auxUsuario = inicioUsuario;
		auxUsuario->ant = nullptr;
		auxUsuario->sig = nullptr;

		strcpy_s(auxUsuario->clave, nuevo->clave);
		strcpy_s(auxUsuario->contra, nuevo->contra);
		strcpy_s(auxUsuario->ApllPat, nuevo->ApllPat);
		strcpy_s(auxUsuario->ApllMat, nuevo->ApllMat);
		strcpy_s(auxUsuario->nombres, nuevo->nombres);
	}
	else {
		auxUsuario = inicioUsuario;
		while (auxUsuario->sig != nullptr) {
			auxUsuario = auxUsuario->sig;
		}
		auxUsuario->sig = new usuario;
		auxUsuario->sig->sig = nullptr;
		auxUsuario->sig->ant = auxUsuario;
		auxUsuario = auxUsuario->sig;

		strcpy_s(auxUsuario->clave, nuevo->clave);
		strcpy_s(auxUsuario->contra, nuevo->contra);
		strcpy_s(auxUsuario->ApllPat, nuevo->ApllPat);
		strcpy_s(auxUsuario->ApllMat, nuevo->ApllMat);
		strcpy_s(auxUsuario->nombres, nuevo->nombres);
	}
	MessageBox(0, "Usuario registrado", "AVISO", MB_OK);
}

void escribirUsuarioBin() {
	auxUsuario = inicioUsuario;
	ofstream escribirUsuarios;
	escribirUsuarios.open("C:\\Users\\mat2k\\source\\repos\\Estructura de Datos 1er Entrega\\Usuarios.bin", ios::out | ios::binary | ios::trunc);
	if (escribirUsuarios.is_open()) {
		while (auxUsuario != nullptr) {
			escribirUsuarios.write((char*)auxUsuario, sizeof(usuario));
			auxUsuario = auxUsuario->sig;
		}
		escribirUsuarios.close();
	}
	else {
		MessageBox(0, "No se pudo abrir el archivo", "AVISO", MB_OK);
	}
}

void leerUsuarioBin() {
	ifstream archivoUsuarios;
	archivoUsuarios.open("C:\\Users\\mat2k\\source\\repos\\Estructura de Datos 1er Entrega\\Usuarios.bin", ios::in | ios::binary);

	if (archivoUsuarios.is_open()) {
		usuario* usuarioLeido = new usuario;
		while (!archivoUsuarios.read((char*)usuarioLeido, sizeof(usuario)).eof()) {
			if (inicioUsuario == nullptr) {
				inicioUsuario = usuarioLeido;
				inicioUsuario->sig = nullptr;
				auxUsuario = inicioUsuario;
			}
			else {
				auxUsuario->sig = usuarioLeido;
				auxUsuario = auxUsuario->sig;
				auxUsuario->sig = nullptr;
			}
			usuarioLeido = new usuario;
		}
		archivoUsuarios.close();
		delete usuarioLeido;
	}
	else {
		MessageBox(0, "No se pudo abrir el archivo", "AVISO", MB_OK);
	}
}

void agregarPersona(persona* nuevo) {
	if (inicioPersona == nullptr) {
		inicioPersona = new persona;
		auxPersona = inicioPersona;
		auxPersona->ant = nullptr;
		auxPersona->sig = nullptr;

		strcpy_s(auxPersona->ApllPat, nuevo->ApllPat);
		strcpy_s(auxPersona->ApllMat, nuevo->ApllMat);
		strcpy_s(auxPersona->nombres, nuevo->nombres);
		strcpy_s(auxPersona->curp, nuevo->curp);
		strcpy_s(auxPersona->rfc, nuevo->rfc);
		strcpy_s(auxPersona->fechaNacimiento, nuevo->fechaNacimiento);
		strcpy_s(auxPersona->calle, nuevo->calle);
		strcpy_s(auxPersona->colonia, nuevo->colonia);
		strcpy_s(auxPersona->ciudad, nuevo->ciudad);
		strcpy_s(auxPersona->estado, nuevo->estado);
		strcpy_s(auxPersona->estadoCivil, nuevo->estadoCivil);
		strcpy_s(auxPersona->telefono, nuevo->telefono);
		strcpy_s(auxPersona->docIdentidad, nuevo->docIdentidad);
		strcpy_s(auxPersona->sexo, nuevo->sexo);
		strcpy_s(auxPersona->gpoOcupacional, nuevo->gpoOcupacional);
		strcpy_s(auxPersona->perfilRiesgo, nuevo->perfilRiesgo);
		strcpy_s(auxPersona->foto, nuevo->foto);
	}
	else {
		auxPersona = inicioPersona;
		while (auxPersona->sig != nullptr) {
			auxPersona = auxPersona->sig;
		}
		auxPersona->sig = new persona;
		auxPersona->sig->sig = nullptr;
		auxPersona->sig->ant = auxPersona;
		auxPersona = auxPersona->sig;

		strcpy_s(auxPersona->ApllPat, nuevo->ApllPat);
		strcpy_s(auxPersona->ApllMat, nuevo->ApllMat);
		strcpy_s(auxPersona->nombres, nuevo->nombres);
		strcpy_s(auxPersona->curp, nuevo->curp);
		strcpy_s(auxPersona->rfc, nuevo->rfc);
		strcpy_s(auxPersona->fechaNacimiento, nuevo->fechaNacimiento);
		strcpy_s(auxPersona->calle, nuevo->calle);
		strcpy_s(auxPersona->colonia, nuevo->colonia);
		strcpy_s(auxPersona->ciudad, nuevo->ciudad);
		strcpy_s(auxPersona->estado, nuevo->estado);
		strcpy_s(auxPersona->estadoCivil, nuevo->estadoCivil);
		strcpy_s(auxPersona->telefono, nuevo->telefono);
		strcpy_s(auxPersona->docIdentidad, nuevo->docIdentidad);
		strcpy_s(auxPersona->sexo, nuevo->sexo);
		strcpy_s(auxPersona->gpoOcupacional, nuevo->gpoOcupacional);
		strcpy_s(auxPersona->perfilRiesgo, nuevo->perfilRiesgo);
		strcpy_s(auxPersona->foto, nuevo->foto);
	}
	MessageBox(0, "Persona registrada", "AVISO", MB_OK);
}

void escribirPersonaBin() {
	auxPersona = inicioPersona;
	ofstream escribirPersonas;
	escribirPersonas.open("C:\\Users\\mat2k\\source\\repos\\Estructura de Datos 1er Entrega\\Personas.bin", ios::out | ios::binary | ios::trunc);
	if (escribirPersonas.is_open()) {
		while (auxPersona != nullptr) {
			escribirPersonas.write((char*)auxPersona, sizeof(persona));
			auxPersona = auxPersona->sig;
		}
		escribirPersonas.close();
	}
	else {
		MessageBox(0, "No se pudo abrir el archivo", "AVISO", MB_OK);
	}
}

void leerPersonaBin() {
	ifstream archivoPersonas;
	archivoPersonas.open("C:\\Users\\mat2k\\source\\repos\\Estructura de Datos 1er Entrega\\Personas.bin", ios::in | ios::binary);

	if (archivoPersonas.is_open()) {
		persona* personaLeida = new persona;
		while (!archivoPersonas.read((char*)personaLeida, sizeof(persona)).eof()) {
			if (inicioPersona == nullptr) {
				inicioPersona = personaLeida;
				inicioPersona->sig = nullptr;
				auxPersona = inicioPersona;
			}
			else {
				auxPersona->sig = personaLeida;
				auxPersona = auxPersona->sig;
				auxPersona->sig = nullptr;
			}
			personaLeida = new persona;
		}
		archivoPersonas.close();
		delete personaLeida;
	}
	else {
		MessageBox(0, "No se pudo abrir el archivo", "AVISO", MB_OK);
	}
}

void agregarVacuna(vacuna* nuevo) {
	if (inicioVacuna == nullptr) {
		inicioVacuna = new vacuna;
		auxVacuna = inicioVacuna;
		auxVacuna->ant = nullptr;
		auxVacuna->sig = nullptr;

		strcpy_s(auxVacuna->tipo, nuevo->tipo);
		strcpy_s(auxVacuna->marca, nuevo->marca);
		strcpy_s(auxVacuna->numDosis, nuevo->numDosis);
		strcpy_s(auxVacuna->claveVacuna, nuevo->claveVacuna);
		strcpy_s(auxVacuna->descripcion, nuevo->descripcion);
		auxVacuna->precio = nuevo->precio;
	}
	else{
		auxVacuna = inicioVacuna;
		while (auxVacuna->sig != nullptr) {
			auxVacuna = auxVacuna->sig;
		}
		auxVacuna->sig = new vacuna;
		auxVacuna->sig->sig = nullptr;
		auxVacuna->sig->ant = auxVacuna;
		auxVacuna = auxVacuna->sig;

		strcpy_s(auxVacuna->tipo, nuevo->tipo);
		strcpy_s(auxVacuna->marca, nuevo->marca);
		strcpy_s(auxVacuna->numDosis, nuevo->numDosis);
		strcpy_s(auxVacuna->claveVacuna, nuevo->claveVacuna);
		strcpy_s(auxVacuna->descripcion, nuevo->descripcion);
		auxVacuna->precio = nuevo->precio;
	}
	MessageBox(0, "Vacuna registrada", "AVISO", MB_OK);
}

void escribirVacunaBin() {
	auxVacuna = inicioVacuna;
	ofstream escribirVacuna;
	escribirVacuna.open("C:\\Users\\mat2k\\source\\repos\\Estructura de Datos 1er Entrega\\Vacunas.bin", ios::out | ios::binary | ios::trunc);
	if (escribirVacuna.is_open()) {
		while (auxVacuna != nullptr) {
			escribirVacuna.write((char*)auxVacuna, sizeof(vacuna));
			auxVacuna = auxVacuna->sig;
		}
		escribirVacuna.close();
	}
	else {
		MessageBox(0, "No se pudo abrir el archivo", "AVISO", MB_OK);
	}
}

void leerVacunasBin() {
	ifstream archivoVacunas;
	archivoVacunas.open("C:\\Users\\mat2k\\source\\repos\\Estructura de Datos 1er Entrega\\Vacunas.bin", ios::in | ios::binary);

	if (archivoVacunas.is_open()) {
		vacuna* vacunasLeido = new vacuna;
		while (!archivoVacunas.read((char*)vacunasLeido, sizeof(vacuna)).eof()) {
			if (inicioVacuna == nullptr) {
				inicioVacuna = vacunasLeido;
				inicioVacuna->sig = nullptr;
				auxVacuna = inicioVacuna;
			}
			else {
				auxVacuna->sig = vacunasLeido;
				auxVacuna = auxVacuna->sig;
				auxVacuna->sig = nullptr;
			}
			vacunasLeido = new vacuna;
		}
		archivoVacunas.close();
		delete vacunasLeido;
	}
	else {
		MessageBox(0, "No se pudo abrir el archivo", "AVISO", MB_OK);
	}
}