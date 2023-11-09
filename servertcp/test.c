//#include<stdio.h>
//int mat[2][3] = {777,777,777,777,777,777};
//void room_ids(int value_room_id,int conn_id){
//    int i,j;
//    for(i=0;i<2;i++)
//        for(j=0;j<3;j++)
//        {
//            if (mat[i][j] == conn_id){
//                mat[i][j] = 777;
//            }
//        }
//    for (i = value_room_id; i == value_room_id; i++) {
//        for (j = 0; j < 3; j++) {
//            if (mat[i][j] == 777) {
//                mat[i][j] = conn_id;
//                break;
//            }
//        }
//    }
//}
//
//int main(void)
//{
//    int conn_index = 4;
//    int room_id = 0;
//    room_ids(room_id,conn_index);
//    printf("\nmatrix mod\n");
//    for (int i = 0; i < 2; i++) {
//        for (int j = 0; j < 3; j++) {
//            printf("%d ", mat[i][j]);
//        }
//        printf("\n");
//    }
//    room_ids(room_id,5);
//    printf("\nmatrix mod\n");
//    for (int i = 0; i < 2; i++) {
//        for (int j = 0; j < 3; j++) {
//            printf("%d ", mat[i][j]);
//        }
//        printf("\n");
//    }
//    room_ids(1,6);
//    room_ids(1,5);
//    printf("\nmatrix mod\n");
//    for (int i = 0; i < 2; i++) {
//        for (int j = 0; j < 3; j++) {
//            printf("%d ", mat[i][j]);
//        }
//        printf("\n");
//    }
//
//    printf("\nlista q eu quero\n");
////    for (int i = 0; i < 2; i++) {
//        for (int j = 0; j < 3; j++) {
//            if (mat[room_id][j] != 777){
//                printf("%d ", mat[room_id][j]);
//            }
//        }
//        printf("\n");
////    }
//
//}




#include <stdio.h>
#include <string.h>

int main() {
    char strings[3][50]; // Matriz de strings com 3 elementos, cada um com espaço para até 50 caracteres

    // Inicialize as strings
    strcpy(strings[0], "Hello, world!");
    strcpy(strings[1], "This is a sample string.");
    strcpy(strings[2], "C programming is fun.");

    // Exiba as strings
    for (int i = 0; i < 3; i++) {
        printf("String %d: %s\n", i, strings[i]);
    }

    return 0;
}
