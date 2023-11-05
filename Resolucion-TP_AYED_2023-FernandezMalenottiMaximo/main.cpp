#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

struct Empleado {
	char codEmp[9 + 1];
	char nombYApe[50 + 1];
	int cantProdVend;
};

struct Venta {
	char codEmp[9 + 1];
	int codProd;
	int fecha;
	float precioVenta;
};

void crearEmpleados() {

	FILE* empleados = fopen("Empleados.dat","wb+");

	Empleado vecE[4] = {
		{ "EE", "Juan Gomez", 50 },
		{ "AA", "Pablo Perez", 100 },
		{ "ZZ", "Ariel Gimenez", 10 },
		{ "NN", "Roberto Diaz", 200 }
	};

	fwrite(vecE, sizeof(Empleado), 4, empleados);
	fclose(empleados);
}

void mostrarEmpleados() {
	FILE* empleados = fopen("Empleados.dat","rb+");

	cout << "Codigo Empleado, Nombre y Apellido, Cantidad de productos Vendidos" << endl;
	Empleado e;
	fread(&e, sizeof(Empleado), 1, empleados);

	while (!feof(empleados)) {
		cout << e.codEmp << ", " << e.nombYApe << ", " << e.cantProdVend << endl;
		fread(&e, sizeof(Empleado), 1, empleados);
	}

	cout << endl;
	fclose(empleados);
}

void crearVentas() {

	FILE* ventas = fopen("Ventas.dat","wb+");

	Venta vecV[11] = {
		{ "EE", 1, 20141001, 40.0 },
		{ "ZZ", 5, 20141001, 30.0 },
		{ "EE", 2, 20141002, 15.0 },
		{ "AA", 5, 20141002, 30.0 },
		{ "AA", 5, 20141002, 30.0 },
		{ "EE", 2, 20141002, 15.0 },
		{ "ZZ", 3, 20141004, 70.0 },
		{ "ZZ", 3, 20141010, 70.0 },
		{ "EE", 2, 20141011, 15.0 },
		{ "ZZ", 4, 20141015, 60.0 },
		{ "ZZ", 5, 20141015, 30.0 }
	};

	fwrite(vecV, sizeof(Venta), 11, ventas);
	fclose(ventas);
}

void mostrarVentas() {
	FILE* ventas = fopen("Ventas.dat","rb+");

	cout << "Codigo Empleado, Codigo Producto, Fecha, Precio de Venta" << endl;
	Venta v;
	fread(&v, sizeof(Venta), 1, ventas);

	while (!feof(ventas)) {
		cout << v.codEmp << ", " << v.codProd << ", " << v.fecha << ", " << v.precioVenta << endl;
		fread(&v, sizeof(Venta), 1, ventas);
	}

	cout << endl;
	fclose(ventas);
}

void resolucionTp(); //Debajo del main
	//TODO completar aquí con la resolución del TP
	// recordar usar la libreria string.h para el manejo de comparación y copia de valores de cadenas
	// funciones útiles para usar: strcmp y stcpy


/*
A partir de este punto, se encuentra todo el codigo escrito con el fin de resolver la problematica presentada.
*/

//Estructuras|variables globales

struct Info{
	int codProd;
	int fecha;
	float precioVenta;
};

struct prodVendidos{
	Info info;
	prodVendidos* sig = NULL;
};

struct nodoVendedor {
	char codEmp[9+1];
	char nombreApellido[50+1];
	int cantProdVendidos;
	float totalRec;
	prodVendidos* listVendido;
};

//funcion necesaria para el qsort
int compararVentas(const void* a, const void* b) {
    const nodoVendedor* vendedorA = (const nodoVendedor*)a;
    const nodoVendedor* vendedorB = (const nodoVendedor*)b;
    // Compara los totales de ventas en orden descendente
	return (vendedorB->totalRec > vendedorA->totalRec) - (vendedorB->totalRec < vendedorA->totalRec);
}

void imprimirVectorEstatico(nodoVendedor vectorEstatico[], int contadorEmp);

void liberarMemoriaVentas(nodoVendedor vectorEstatico[], int contadorEmp);

int main(void) {
	crearEmpleados();
	mostrarEmpleados();
	crearVentas();
	mostrarVentas();

	resolucionTp();


	return 0;
}

void resolucionTp() {



	nodoVendedor vectorEstatico[50];

	//Carga de empleados desde Empleados.dat

	FILE* fileEmpleados = fopen("Empleados.dat","rb+");
	Empleado lecEmp;//lectruaEmpleado
	fread(&lecEmp, sizeof(Empleado), 1, fileEmpleados);

	int contadorEmp = 0;
	while (!feof(fileEmpleados)) {
		//carga los vendedores en el vector estatico con el orden del archivo
		strcpy (vectorEstatico[contadorEmp].codEmp, lecEmp.codEmp);
		strcpy (vectorEstatico[contadorEmp].nombreApellido, lecEmp.nombYApe);
		vectorEstatico[contadorEmp].cantProdVendidos = lecEmp.cantProdVend;

		fread(&lecEmp, sizeof(Empleado), 1, fileEmpleados);

		//cout << lecEmp.codEmp << ", " << lecEmp.nombYApe << ", " << lecEmp.cantProdVend << endl;
		//cout<<vectorEstatico[contadorEmp].codEmp<<"|"<<vectorEstatico[contadorEmp].nombreApellido<<"|"<<vectorEstatico[contadorEmp].cantProdVendidos<<endl;
		contadorEmp ++;
	}
	fclose(fileEmpleados);

		// Carga de ventas desde Ventas.dat
    FILE* fileVentas = fopen("Ventas.dat", "rb+");
    Venta lecVen;//lecturaVenta
    fread(&lecVen, sizeof(Venta), 1, fileVentas);
    cout<<lecVen.codEmp<<"|"<<lecVen.precioVenta<<endl;
    while (!feof(fileVentas)) {
        // Encuentra el empleado correspondiente
        for (int i = 0; i < contadorEmp; i++) {//contadorEmp conseva la cantidad de empleados cargados
           if (strcmp(lecVen.codEmp, vectorEstatico[i].codEmp) == 0) {
                cout<<lecVen.codEmp<<"|"<<lecVen.precioVenta<<endl;
                // Crea un nuevo nodo para la venta
                prodVendidos* nuevaVenta = new prodVendidos;
                nuevaVenta->info.codProd = lecVen.codProd;
                nuevaVenta->info.fecha = lecVen.fecha;
                nuevaVenta->info.precioVenta = lecVen.precioVenta;

                // Agrega la venta a la pila (lista enlazada) del empleado
                nuevaVenta->sig = vectorEstatico[i].listVendido;
				vectorEstatico[i].listVendido = nuevaVenta;

                // Calcula el total recaudado
                vectorEstatico[i].totalRec += lecVen.precioVenta;
                break;
            }
        }
        fread(&lecVen, sizeof(Venta), 1, fileVentas);
    }
    fclose(fileVentas);

	//Ordenamiento del vector estatico en orden decendente utilizando qsort (stdlib.h)
	qsort(vectorEstatico, contadorEmp, sizeof(nodoVendedor), compararVentas);

	imprimirVectorEstatico(vectorEstatico, contadorEmp);
	liberarMemoriaVentas(vectorEstatico, contadorEmp);

}


void imprimirVectorEstatico(nodoVendedor vectorEstatico[], int contadorEmp) {
    cout << "Codigo Empleado, Nombre y Apellido, Cantidad de productos Vendidos, Total de Ventas" << endl;

    for (int i = 0; i < contadorEmp; i++) {
        cout << vectorEstatico[i].codEmp << ", "
             << vectorEstatico[i].nombreApellido << ", "
             << vectorEstatico[i].cantProdVendidos << ", "
             << vectorEstatico[i].totalRec << endl;

        // También puedes imprimir las ventas de cada empleado aquí
        prodVendidos* nActual = vectorEstatico[i].listVendido;
        cout << "Ventas:" << endl;
        while (nActual != NULL) {
            cout << "Codigo Producto: " << nActual->info.codProd << ", Fecha: " << nActual->info.fecha << ", Precio: " << nActual->info.precioVenta << endl;
            nActual = nActual->sig;
        }

        cout << endl;
    }
}

void liberarMemoriaVentas(nodoVendedor vectorEstatico[], int contadorEmp) {
    for (int i = 0; i < contadorEmp; i++) {
        prodVendidos* nActual = vectorEstatico[i].listVendido;
        while (nActual != NULL) {
            prodVendidos* nodoAEliminar = nActual;
            nActual = nActual->sig;
            delete nodoAEliminar;  // Libera la memoria del nodo
        }
    }
}
