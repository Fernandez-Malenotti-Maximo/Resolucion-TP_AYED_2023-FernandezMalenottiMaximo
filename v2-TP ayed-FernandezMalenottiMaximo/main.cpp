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

//estructuras--------------------------------------------------------------------------

struct Info{
	int codProd;
	int fecha;
	float precioVenta;
};

struct nodoProdVendidos{
	Info info;
	nodoProdVendidos* sig = NULL;
};

struct Vendedor {
	char codEmp[9+1];
	char nombreApellido[50+1];
	int cantProdVendidos;
	float totalRec;
	nodoProdVendidos* listVendido;
};

//funciones----------------------------------------------------------------------------

bool cargaVendedores (Vendedor* vectorVendedores, int& contadorEmp);   //carga los vendedores desde el archivo al vector
bool cargaVentas (Vendedor* vectorVendedores, int contadorEmp);          //carga las ventas correspondientes a cada vendedor
bool imprimirDatos (Vendedor* vectorVendedores, int contadorEmp);                           //imprime los datos existentes en el vector
int compararVentasPorFechaDesc(const void* a, const void* b);               //funcion de ordenamiento para qsort
int compararVendedores(const void* a, const void* b);                       //funcion de ordenamiento para qsort


bool cargaVendedores(Vendedor* vectorVendedores, int& contadorEmp){

    FILE* archVendedores = fopen("Empleados.dat","rb+");
    Empleado lecEmpleado;
    fread(&lecEmpleado, sizeof(Empleado), 1, archVendedores);
    contadorEmp = 0;

    while (!feof(archVendedores)) {

		//carga los vendedores en el vector estatico con el orden del archivo
		strcpy (vectorVendedores[contadorEmp].codEmp, lecEmpleado.codEmp);
		strcpy (vectorVendedores[contadorEmp].nombreApellido, lecEmpleado.nombYApe);
		vectorVendedores[contadorEmp].cantProdVendidos = lecEmpleado.cantProdVend;

		fread(&lecEmpleado, sizeof(Empleado), 1, archVendedores);

		contadorEmp ++;
    }
    fclose(archVendedores);
    return true;
}

bool cargaVentas (Vendedor* vectorVendedores, int contadorEmp){

    FILE* archVentas = fopen("Ventas.dat", "rb+");
    Venta lecVentas;//lecturaVenta
    fread(&lecVentas, sizeof(Venta), 1, archVentas);

    while (!feof(archVentas)) {
        // Encuentra el empleado correspondiente

        for (int i = 0; i < contadorEmp; i++) {//contadorEmp conseva la cantidad de empleados cargados
           if (strcmp(lecVentas.codEmp, vectorVendedores[i].codEmp) == 0) {
                // Crea un nuevo nodo para la venta
                nodoProdVendidos* nuevaVenta = new nodoProdVendidos;//genero una venta y reclamo el espacio en memoria
                nuevaVenta->info.codProd = lecVentas.codProd;//cargo en el cod de la nueva venta el dato leido
                nuevaVenta->info.fecha = lecVentas.fecha;
                nuevaVenta->info.precioVenta = lecVentas.precioVenta;
                nuevaVenta->sig = NULL;

                // Agrega la venta a la pila (lista enlazada) del empleado
                cout<<"direccion de nueva venta sig: "<<nuevaVenta <<endl;
				cout<<"direccion de primer venta: "<<vectorVendedores[i].listVendido <<endl;

				nuevaVenta->sig = vectorVendedores[i].listVendido;
				vectorVendedores[i].listVendido = nuevaVenta;

				cout<<"direccion de nueva venta sig: "<<nuevaVenta->sig <<endl;
				cout<<"direccion de primer venta"<<vectorVendedores[i].listVendido<<endl<<endl;


                // Calcula el total recaudado
                vectorVendedores[i].totalRec += lecVentas.precioVenta;
                break;
            }
        }
        fread(&lecVentas, sizeof(Venta), 1, archVentas);
    }


return true;
}

int compararVendedores(const void* a, const void* b) {
    const Vendedor* vendedorA = static_cast<const Vendedor*>(a);
    const Vendedor* vendedorB = static_cast<const Vendedor*>(b);

    // Compara por el totalRec en orden ascendente
    if (vendedorA->totalRec < vendedorB->totalRec) {
        return -1;
    } else if (vendedorA->totalRec > vendedorB->totalRec) {
        return 1;
    }
    return 0;
}

int compararVentasPorFechaDesc(const void* a, const void* b) {
    const nodoProdVendidos* ventaA = static_cast<const nodoProdVendidos*>(a);
    const nodoProdVendidos* ventaB = static_cast<const nodoProdVendidos*>(b);

    // Compara las fechas en orden descendente
    return (ventaB->info.fecha > ventaA->info.fecha);
}


bool imprimirDatos (Vendedor* vectorVendedores, int contadorEmp){

    // Ordena los vendedores en orden ascendente según lo recaudado
    qsort(vectorVendedores, contadorEmp, sizeof(Vendedor), compararVendedores);

    for (int i = 0; i <= contadorEmp; i++) {
        cout
        << "Código Empleado: " << vectorVendedores[i].codEmp << endl
        << "Nombre y Apellido: "<< vectorVendedores[i].nombreApellido << endl
        << "Cantidad de productos Vendidos: "<< vectorVendedores[i].cantProdVendidos << endl
        << "Total de Ventas: " << vectorVendedores[i].totalRec << endl<<endl;

        // imprime las ventas de cada vendedor
        if (vectorVendedores[i].listVendido != NULL) {
            int cantVentas=1;
            while(vectorVendedores[i].listVendido!= NULL){
                cout<<"cod prod: "<<vectorVendedores[i].listVendido->info.codProd<<endl;
                cout<<"fecha: "<<vectorVendedores[i].listVendido->info.fecha<<endl;
                cout<<"precio de venta: "<<vectorVendedores[i].listVendido->info.precioVenta<<endl<<endl;

                if(vectorVendedores[i].listVendido != NULL){vectorVendedores[i].listVendido = vectorVendedores[i].listVendido ->sig;}

                cout<<vectorVendedores[i].listVendido<<endl;
                cout<<cantVentas<<endl;
                cantVentas++;

            }
            cout<<"dentro del if"<<endl;
        }

    cout<<"antes de finalizar el bucle"<<endl;

    }

    return true;
}



//resolucion---------------------------------------------------------------------------
void resolucionTp() {

    Vendedor vectorVendedores[50];
    int contadorEmp = 0;//cant de empleados cargados

    if (cargaVendedores(vectorVendedores, contadorEmp)) {

        if (cargaVentas(vectorVendedores, contadorEmp)) {

            imprimirDatos(vectorVendedores, contadorEmp);

        }
    }
}

int main() {
	crearEmpleados();
	mostrarEmpleados();
	crearVentas();
	mostrarVentas();

	resolucionTp();

	return 0;
}
