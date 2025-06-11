/*
 * Programa: Gestor de precios para emprendimientos
 * Autor: Elias Jesus Garcia
 * Descripción: Permite crear, editar y visualizar listas de precios con persistencia en archivos binarios.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INCREMENTO_PORCENTUAL 0.5

struct Producto
{
    char nombre[30];
    int precio;
};

void muestraPrecios()
{
    struct Producto X;
    int i = 1;
    FILE *FP;

    if(!(FP = fopen("preciosSN.bin", "r+b")))
    {
        printf("HUBO ERROR AL LEER ARCHIVO. no existe");
        return;
    }

    printf("\nLISTA DE PRECIOS\n");
    while(fread(&X, sizeof(X), 1, FP) == 1)
    {
        printf("\n\t%d. %s..........%d", i, X.nombre, X.precio);
        i++;
    }

    getchar();
    fclose(FP);
}

void cargaPrecios(int PRECIOBOLSA)
{
    int i, opcion, costo;
    struct Producto X;
    FILE *FP;

    FP = fopen("preciosSN.bin", "wb");

    if(FP == NULL)
    {
        printf("Error al crear o abrir archivo");
        exit(1);
    }

    printf("\nIngrese 1 para cargar datos");
    printf("\nIngrese 0 para cerrar modificacion\n");

    while(scanf("%d", &opcion) != 1 || opcion > 1 || opcion < 0)
    {
        while(getchar() != '\n'); // Limpieza del buffer
        printf("\nIngreso una opcion incorrecta. Reingresar\n");
    }

    while(opcion == 1)
    {
        while(getchar() != '\n'); // Limpieza del buffer

        printf("\nIngrese nombre del producto: ");
        fgets(X.nombre, sizeof(X.nombre), stdin);
        X.nombre[strcspn(X.nombre, "\n")] = '\0'; // Eliminar el salto de línea

        printf("\nIngrese costo del producto para calcular precio: ");
        scanf("%d", &costo);

        X.precio = (int)((costo + PRECIOBOLSA) * (1 + INCREMENTO_PORCENTUAL)) ;

        fwrite(&X, sizeof(X), 1, FP);

        printf("\nIngrese 1 para cargar datos");
        printf("\nIngrese 0 para cargar cerrar modificacion\n");

        while(scanf("%d", &opcion) != 1 || opcion > 1 || opcion < 0)
        {
            while(getchar() != '\n'); // Limpieza del buffer
            printf("\nIngreso una opcion incorrecta. Reingresar\n");
        }
    }

    fclose(FP);
    muestraPrecios();
}

void agregaProd(int PRECIOBOLSA)
{
    struct Producto AX;
    int opt;
    FILE *FP;

    if(!(FP = fopen("preciosSN.bin", "ab")))
    {
        printf("\nERROR AL ABRIR preciosSN.bin\n");
        exit(1);
    }

    printf("\nSE AGREGARAN PRODUCTOS NUEVOS AL FINAL DE LA LISTA\n");

    printf("\nIngrese 1 para agregar producto\n Ingrese 0 para salir\n");

    while(scanf("%d", &opt) != 1 || opt < 0 || opt > 1)
    {
        while(getchar() != '\n'); // Limpieza del buffer
        printf("\nSe ingreso una opcion incorrecta \n\tREINGRESE.\n");
        printf("\nIngrese 1 para agregar producto\n Ingrese 0 para salir\n");
    }

    while(opt == 1)
    {
        while(getchar() != '\n'); // Limpieza del buffer

        printf("\nIngrese nombre de producto nuevo:");
        fgets(AX.nombre, sizeof(AX.nombre), stdin);
        AX.nombre[strcspn(AX.nombre, "\n")] = '\0'; // Eliminar el salto de línea

        printf("\nIngrese costo del producto ");
        puts(AX.nombre);
        scanf("%d", &AX.precio);

        AX.precio = (int)((AX.precio + PRECIOBOLSA) * (1 + INCREMENTO_PORCENTUAL));

        fwrite(&AX, sizeof(AX), 1, FP);

        printf("\nIngrese 1 para agregar producto\n Ingrese 0 para salir\n");

        while(scanf("%d", &opt) != 1 || opt < 0 || opt > 1)
        {
            while(getchar() != '\n'); // Limpieza del buffer
            printf("\nSe ingreso una opcion incorrecta \n\tREINGRESE.\n");
            printf("\nIngrese 1 para agregar producto\n Ingrese 0 para salir\n");
        }
    }

    printf("\nCerrando archivo...\n");
    getchar();

    fclose(FP);
}

void modProd(int PRECIOBOLSA)
{
    struct Producto MX;
    int buscar, n, nuevo_costo;
    FILE *FPM;

    if(!(FPM = fopen("preciosSN.bin", "r+b")))
    {
        printf("\nERROR AL CREAR O ABRIR ARCHIVO");
        exit(1);
    }

    fseek(FPM, 0L, SEEK_END);
    n = ftell(FPM)/sizeof(MX);

    printf("\nIngrese numero de lista del producto:");

    while(scanf("%d", &buscar) != 1 || buscar < 0 || buscar > n)
    {
        while(getchar() != '\n'); // Limpieza del buffer
        printf("\nINGRESO INVALIDO \n(no se admiten caracteres o numeros que NO esten en la lista)\n");
        printf("\nIngrese numero de lista del producto nuevamente:");
    }

    if (n == 0) {
        printf("\nNo hay productos para modificar.\n");
        fclose(FPM);
        return;
    }
    if (buscar == 0 && n > 0) {
        printf("\nNúmero de lista inválido, debe ser mayor a 0.\n");
        fclose(FPM);
        return;
    }

    buscar--;

    fseek(FPM, (long) buscar*sizeof(MX), SEEK_SET);
    fread(&MX, sizeof(struct Producto), 1, FPM);

    printf("\nSE MODIFICARA EL PRODUCTO ");
    puts(MX.nombre);
    printf("\nEl precio actual es %d", MX.precio);

    while(getchar() != '\n'); // Limpieza del buffer

    printf("\nINGRESE NUEVO NOMBRE: ");
    fgets(MX.nombre, sizeof(MX.nombre), stdin);
    MX.nombre[strcspn(MX.nombre, "\n")] = '\0'; // Eliminar el salto de línea

    printf("\nINGRESE NUEVO COSTO: ");
    scanf("%d", &nuevo_costo);

    MX.precio = (int)((nuevo_costo + PRECIOBOLSA) * (1 + INCREMENTO_PORCENTUAL)) ;

    fseek(FPM, (long)-sizeof(MX), SEEK_CUR);

    fwrite(&MX, sizeof(MX), 1, FPM);

    fclose(FPM);
}

int main()
{
    int opcion;
    int PRECIOBOLSA;

    printf("\n\n\t\tESTE ES EL PROGRAMA DE GESTION DE PRECIOS\n");

    printf("\nIngrese el precio del packaging (BOLSA O ZIPLOC):\n");
    while(scanf("%d", &PRECIOBOLSA)!=1){
    printf("\n\tINCORRECTO. REINGRESE MONTO\n");
    while(getchar() != '\n');
    }

    printf("\nIngrese una opcion: \n");
    printf("\n1-----MOSTRAR LISTA ACTUAL");
    printf("\n2-----CREAR LISTA NUEVA (se perdera la actual)");
    printf("\n3-----AGREGAR PRODUCTO A LA LISTA ACTUAL");
    printf("\n4-----MODIFICAR NOMBRE O PRECIO DE PRODUCTO");
    printf("\n0-----CERRAR PROGRAMA\n");

    while(scanf("%d", &opcion) != 1 || opcion > 4 || opcion < 0)
    {
        while(getchar() != '\n'); // Limpieza del buffer
        printf("\nINGRESO UN NUMERO O CARACTER INCORRECTO... REINGRESAR A CONTINUACION: \n");
        printf("\nIngrese una opcion: \n");
        printf("\n1-----MOSTRAR LISTA ACTUAL");
        printf("\n2-----CREAR LISTA NUEVA (se perdera la actual)");
        printf("\n3-----AGREGAR PRODUCTO A LA LISTA ACTUAL");
        printf("\n4-----MODIFICAR NOMBRE O PRECIO DE PRODUCTO");
        printf("\n0-----CERRAR PROGRAMA\n");
    }

    while(opcion != 0)
    {
        switch(opcion)
        {
        case 1:
            muestraPrecios();
            break;

        case 2:
            cargaPrecios(PRECIOBOLSA);
            break;

        case 3:
            agregaProd(PRECIOBOLSA);
            break;

        case 4:
            modProd(PRECIOBOLSA);
            break;
        }


        printf("\nIngrese una opcion: \n");
        printf("\n1-----MOSTRAR LISTA ACTUAL");
        printf("\n2-----CREAR LISTA NUEVA (se perdera la actual)");
        printf("\n3-----AGREGAR PRODUCTO A LA LISTA ACTUAL");
        printf("\n4-----MODIFICAR NOMBRE O PRECIO DE PRODUCTO");
        printf("\n0-----CERRAR PROGRAMA\n");

        while(scanf("%d", &opcion) != 1 || opcion > 4 || opcion < 0)
        {
            while(getchar() != '\n'); // Limpieza del buffer
            printf("\nINGRESO UN NUMERO O CARACTER INCORRECTO... REINGRESAR A CONTINUACION: \n");
            printf("\nIngrese una opcion: \n");
            printf("\n1-----MOSTRAR LISTA ACTUAL");
            printf("\n2-----CREAR LISTA NUEVA (se perdera la actual)");
            printf("\n3-----AGREGAR PRODUCTO A LA LISTA ACTUAL");
            printf("\n4-----MODIFICAR NOMBRE O PRECIO DE PRODUCTO");
            printf("\n0-----CERRAR PROGRAMA\n");
        }
    }

    printf("\n\nEl programa finalizo");
    printf("\n\t\tDev: Elias Jesus Garcia\n\n");

    system("pause");
    return 0;
}
