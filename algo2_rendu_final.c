//ICHOU Aymane
#include <stdlib.h>
#include <stdio.h>
#include <regex.h> 
#include <assert.h>
#include <string.h>

char _motif_1[]= "CH*MI+" ;
char _motif_2[]= "CH?M*I" ;

void affiche_un_motif (char *str, int deb, int fin);
void affiche_motifs(int *taboccur, int indocc, int nbocc, char *str);
/*--------------Struct------------------*/
struct Motif {
    int debut ; //indice du debut de motif
    int fin ; // indice de fin de motif 
    char * tab;
    int est_dernier ;
    int nb_doublons ; //initialisé à 0 , si ce n'est pas calculé 
};
typedef struct Motif Motif_t; 

struct Tuple{
    int indice ; 
    int booleen ; 
};
typedef struct Tuple Tuple_t;

//Prends un tableau de Motif_t (Motif_t *) et retourne sa taille  
int taille_tab(Motif_t * tab_motif){
    if(tab_motif == NULL){
        printf("ERREUR POINTEUR");
        exit(1) ;  
    }
    int taille = 0 ; 
    while(tab_motif[taille].est_dernier == 0){
        taille++ ;
    }
    return taille; 
}
//Générateur de tableau de lettre de départ avec l'alphabet CHMI 
char * gen_tab(int taille){ 
    char tab[4] = {'C' , 'H' , 'M' , 'I' }; 
    char * retour = malloc(sizeof(char) * taille +1);
    assert(retour); 
    int i , random ; 
    for(i=0 ; i< taille-1 ; i++) {
        random = rand() % 4 ; 
        retour[i] = tab[random] ; 
    }
    retour[taille-1] = '\0' ; 
    return retour ; 
} 

//Libérateur de mémoire d'un tableau de type Motif_t 
void free_tab_occ(Motif_t * tableau){
    int i;
    
    for(i=0; tableau[i].est_dernier==0 ; i++){
        if(tableau[i].tab !=NULL)
       free(tableau[i].tab);

    } 
    if(tableau != NULL)
    free(tableau);
}

//Afficher un tableau de Motif_t de la forme "xxyz [1 4] avec 1 occurences trouvés"
void afficher_tab_occ(Motif_t * tableau){
    int i;
    printf("Tableau des occurences: \n");
    for(i=0; tableau[i].est_dernier==0 ; i++){
        printf("%s avec %d occurences trouvée(s)\n", tableau[i].tab , tableau[i].nb_doublons);
    } 
    printf("%d motifs unique trouvés.\n", i);
    printf("--------------------\n");
}

//Afficher un tableau de Motif_t de la forme "xxyz [1 4] avec 1 occurences trouvés"
void afficher_tab_sans_occ(Motif_t * tableau){
    int i;
    printf("Tableau des occurences: \n");
    for(i=0; tableau[i].est_dernier==0 ; i++){
        printf("%s  [ %d      %d ] \n", tableau[i].tab , tableau[i].debut, tableau[i].fin);
    } 
    printf("%d motifs trouvés.\n", i);
    printf("--------------------\n");
}

//Renvoie si une chaine de caractere est dans un tableau de Motif_t ou non, deux cas possibles : 
// Dans un Tuple de deux éléments de deux éléments sera renvoyé : 
// Si c'est dedans : l'indice de l'element dans le tableau, un booleen = 1 
// Sinon : Indice = -1 et booleen = 0   
Tuple_t est_dedans(Motif_t * tab_motif , char * chaine){
    int i ; 
    Tuple_t temp; 
    int taille= taille_tab(tab_motif) ; 
    for(i = 0 ; i< taille ; i++){
        if(strcmp(tab_motif[i].tab, chaine) == 0){
            temp.indice = i; 
            temp.booleen = 1 ; 
            return temp ; 
        }
    }
    temp.indice = -1 ; 
    temp.booleen = 0 ; 
    return temp ; 
}

//Fonction qui va retourner un tableau de motif_t des occurences trouvés 
Motif_t * search_motif(char * tab_lettre, const char * motif_a_trouver){
    // Créer une structure regex_t pour stocker le motif compilé
    int i, j;
    int nb_elem = 0;
    int decalage = 0;
    int nb_elem_max = 1000;
    Motif_t * tab_motif;
    tab_motif = malloc(nb_elem_max * sizeof(Motif_t));

    regmatch_t match;
    regex_t regex;
    int to_find = regcomp(&regex, motif_a_trouver, REG_EXTENDED);
    if(to_find != 0) 
        printf("Erreur REGEX \n");

    while (regexec(&regex, tab_lettre, 1, &match, 0) == 0) {
        if (nb_elem >= nb_elem_max) {
            nb_elem_max *= 2;
            tab_motif = realloc(tab_motif, sizeof(Motif_t)*nb_elem_max);
            assert(tab_motif);
        }
        tab_motif[nb_elem].tab = malloc((match.rm_eo - match.rm_so + 1) * sizeof(char));
        tab_motif[nb_elem].debut = match.rm_so + decalage;
        tab_motif[nb_elem].fin = match.rm_eo + decalage - 1;
        for(i=0; i<(match.rm_eo-match.rm_so); i++){
            tab_motif[nb_elem].tab[i] = tab_lettre[match.rm_so+i];
        }
        tab_motif[nb_elem].est_dernier = 0;
        tab_motif[nb_elem].nb_doublons = 1; 
        tab_motif[nb_elem].tab[i] = '\0';
        tab_lettre += match.rm_eo;
        decalage+=match.rm_eo;
        nb_elem++;
        }
    tab_motif[nb_elem].est_dernier = 1;
    regfree(&regex);
    return tab_motif;
}


Motif_t * union_motif(Motif_t * tableau1 , Motif_t * tableau2 ){
    int taille_tableau1 = taille_tab(tableau1) ;
    int taille_tableau2 = taille_tab(tableau2) ; 
    int taille_tableau3 = taille_tableau2 + taille_tableau1 ; 
    Motif_t * retour = malloc(sizeof(Motif_t) * (taille_tableau3+1)) ; 
    assert(retour) ;  
    retour[0].est_dernier = 1 ; 
    int i ; 
    for(i=0; i<taille_tableau3; i++) retour[i].nb_doublons = 0;
    int compt_elem = 0 ; 
    Tuple_t res_func_est_dedans ; 
    for(i = 0 ; i< taille_tableau1 ; i++ ){
        res_func_est_dedans = est_dedans(retour , tableau1[i].tab) ; 
        if( res_func_est_dedans.booleen == 0 ){
            retour[compt_elem].debut = tableau1[i].debut ; 
            retour[compt_elem].fin = tableau1[i].fin;
            retour[compt_elem].tab = malloc(((tableau1[i].fin - tableau1[i].debut) +2) * sizeof(char)); 
            strcpy(retour[compt_elem].tab, tableau1[i].tab) ;
            retour[compt_elem].est_dernier = 0; 
            retour[compt_elem].nb_doublons++;
            //printf("NB_DOUBLONS = %d", retour[compt_elem].nb_doublons);
            compt_elem++ ; 
        }
        if(res_func_est_dedans.booleen == 1){
            retour[res_func_est_dedans.indice].nb_doublons++;
        }
    }
    for(i = 0 ; i< taille_tableau2 ; i++ ){
        res_func_est_dedans = est_dedans(retour ,tableau2[i].tab) ;
        if( res_func_est_dedans.booleen == 0 ){
            retour[compt_elem].debut = tableau2[i].debut ; 
            retour[compt_elem].fin = tableau2[i].fin;
            retour[compt_elem].tab = malloc(15* sizeof(char)); 
            strcpy(retour[compt_elem].tab, tableau2[i].tab) ;
            retour[compt_elem].est_dernier = 0; 
            retour[compt_elem].nb_doublons++;
            compt_elem++ ;  
        }
        if(res_func_est_dedans.booleen == 1){
            //printf("On est ici\n");
            //printf("valeur de res_func_est_dedans = %d \n", res_func_est_dedans.indice);
            retour[res_func_est_dedans.indice].nb_doublons++;
        }
    }
    retour[compt_elem].est_dernier = 1 ; 
    return retour;
}

//Etape 5 : Classement par ordre décroissant selon le nombre d'occurences ----------- 

void swap(Motif_t *x, Motif_t *y) {
    Motif_t temp = *x;
    *x = *y;
    *y = temp;
}

void tri_decroissant(Motif_t * tab) {
    int i, j, indice_val_min;
    int n = taille_tab(tab) ; 
    for (i = 0; i < n-1; i++) {
        indice_val_min = i;
        for (j = i+1; j < n; j++) {
            if (tab[j].nb_doublons > tab[indice_val_min].nb_doublons) {
                indice_val_min = j;
            }
        }
        swap(&tab[indice_val_min], &tab[i]);
    }
}

// Etape 6 : Vérification du bon fonctionnement quant aux occurences trouvés avec tableau statique ----- 
void test_fonc_search_motif(char * lettre, char * _motif_test){
    //Exemple présent sur le sujet 
    printf("-------Résultat attendu de la fonction search_motif :-----------\n");
    printf("wz[ 1  2] \n") ; 
    printf("wxyz[ 4  7] \n") ; 
    printf("wyyz[ 48  51] \n") ; 
    printf("wyz[ 65  67] \n") ; 
    printf("wz[ 69  70] \n") ; 
    printf("wz[ 79  80] \n") ; 
    printf("wz[ 96  97] \n") ; 
    printf("wxz[ 118  120] \n") ; 
    printf("wz[ 132  133] \n") ;
    printf("wxyyyz[ 145  150] \n") ; 
    printf("wz[ 180  181] \n") ; 
    printf("wxz[ 189  191] \n") ; 
    printf("wxz[ 195  197] \n") ; 
    printf("Soit un total de 13 motifs\n");

    printf("-------Résultat obtenu avec la fonction search_motif :-----------\n");
    Motif_t * test = search_motif(lettre , _motif_test); 
    afficher_tab_sans_occ(test) ; 
    free_tab_occ(test) ; 
}

//Etape 7 : Vérification du bon fonctionnement de la fonction faisant l'union de deux tableaux de type Motif_t
//par le biais de tableau statique et de résultat connu à l'avance. 
void test_fonc_union_motif(char * lettre){
    char _motif_test_1[] = "wx?y*z";
    char _motif_test_2[] = "wx+y?z";
    Motif_t * test_1 = search_motif(lettre , _motif_test_1);
    Motif_t * test_2 = search_motif(lettre , _motif_test_2);
    printf("AFFICHAGE DE TEST 1 ET 2 \n");
    printf("----------Test 1----------\n");
    afficher_tab_sans_occ(test_1); 
    printf("----------Test 2----------\n");
    afficher_tab_sans_occ(test_2);
    Motif_t * union_test = union_motif(test_1, test_2) ; 
    printf("-------Résultat attendu de la fonction union_motif :-----------\n");
    printf("Union des motifs trouves sans doublons\n");
    printf("wxxyz 1\n");
    printf("wxyz 2\n");
    printf("wyyz 1\n");
    printf("wxz 6\n");
    printf("wyz 1\n");
    printf("wz 6\n");
    printf("wxyyyz 1\n");
    printf("--> 7 motifs uniques\n");
    printf("-------Résultat obtenu avec la fonction union_motif:-----------\n");
    afficher_tab_occ(union_test) ; 
    free_tab_occ(test_1); 
    free_tab_occ(test_2); 
    free_tab_occ(union_test); 
}

//Etape 8 : Vérification du bon fonctionnement de la fonction tri_decroissant en prennant un 
//tableau qui a été trié de manière décroissante.  
void test_fonc_tri_decroissant(Motif_t * tab){
    int taille = taille_tab(tab) ;
    int i; 
    if(taille <= 1){
        printf("Résultat du test de tri décroissant : RESULTAT CORRECT\n");
        return ; 
    }
    for(i=1 ; i<taille-1 ; i++){
        if(tab[i].nb_doublons< tab[i+1].nb_doublons){
            printf("Résultat du test de tri décroissant : RESULTAT INCORRECT \n");
            return;
        }
    }
    printf("Résultat du test de tri décroissant : RESULTAT CORRECT\n");
}

//Etape 9 : Ajout et adaptation de fonction 

void afficher_int_tab(int * tab ){ 
    for(int i = 0 ; tab[i] != -1 ; i+=2 ) 
    printf("%d %d\n",tab[i] , tab[i+1]) ; 
    printf("\n");
}

//Prends deux tableau de motifs
//Prends leurs indices de début et de fin 
//Les concatene , l'un à la suite de l'autre 
//Et renvoie un tableau d'int 
int * convert_int_tab(Motif_t * test_1 ,Motif_t *  test_2 ){
    int taille_test_1 = taille_tab(test_1); 
    int taille_test_2 = taille_tab(test_2); 
    int taille_total = taille_test_1 + taille_test_2 ; 
    taille_total *= 2 ; 
    int i,j; 

    int * result = malloc(sizeof(int) * (taille_total + 1) ) ; // Ajout de -1 à la fin pour fin du tableau

    //Copie tableau test 1 dans result 
    for(i= 0 ; i< taille_test_1; i++){
        result[i*2] = test_1[i].debut ; 
        result[(i*2)+1] = test_1[i].fin ; 
        //printf("%d \n%d \n", result[i*2], result[(i*2)+1] ); 
    }
    i = i*2 ; 
    //Copie tableau test_2 dans result 
    for(j=0 ; j< taille_test_2 ; j++){
        result[i + (j*2)] = test_2[j].debut ; 
        result[i + (j*2)+1] = test_2[j].fin ; 
        //printf("indice de result = %d \n",i + (j*2)+1);
        //printf("%d \n%d \n", result[i + (j*2)], result[i + (j*2)+1] ); 
    }
    j = j*2 ; 
    result[j+i] = -1 ; 
    afficher_int_tab(result) ;
    return result; 
}

int compte_motif(Motif_t * tab_motif){
        int compteur = 0; 
        int i;
        int taille = taille_tab(tab_motif) ;
        for(i=0 ; i< taille ; i++)
            compteur += tab_motif[i].nb_doublons;
        return compteur;
}


void affiche_motifs(int *taboccur, int indocc, int nbocc, char *str){
    if (indocc >= nbocc*2){
        printf("\n");
        return;
    }
    affiche_un_motif(str, taboccur[indocc], taboccur[indocc+1]);
    affiche_motifs(taboccur, indocc+2, nbocc, str);
}

void affiche_un_motif (char *str, int deb, int fin){
    if(deb>fin){
        printf("   ");
        return;
    }
    printf("%c", str[deb]);
    affiche_un_motif (str, deb+1, fin);
}

int main(){

    //Espace déclaration et initialisation de variables&tableaux. 
    int taille = 200 ;  
    char * tab_lettre = gen_tab(taille); 
    //afficher_tab(tab_lettre , taille); 
    char lettre_test[] = "wwzwwxyzxzzxyyyyxxxyzyzxzywxyxyyyxzyyxxwwxxyzzxwwyyzwyxwwxxyyzxwxwyzxwzyxwwwwxwwzywwwxwxyxzwxwxywzxyzxwwxwwxywyxzyxzzxwxzwxwwyyxyzzxwzyyzyzxzxzyzwxyyyzxyxxyzzzxyyzzxxxxzwyyyxwxyywwwzxywywwxwxzywywxzxx" ; 
    char _motif_test_1[] = "wx?y*z";
    char _motif_test_2[] = "wx+y?z"; 

    //Recherche puis affichage des occurences du premier motif 
    Motif_t * mes_mots_motif_1 = search_motif(tab_lettre , _motif_1) ; 
    afficher_tab_sans_occ(mes_mots_motif_1);

    //Recherche puis affichage des occurences du deuxieme motif 
    Motif_t * mes_mots_motif_2 = search_motif(tab_lettre , _motif_2) ; 
    afficher_tab_sans_occ(mes_mots_motif_2);
     
    //Union des deux premiers motifs + calculs du nombres d'occurences, puis affichage avec le nombre d'occurence
    Motif_t * union_des_motif = union_motif(mes_mots_motif_1,mes_mots_motif_2);
    afficher_tab_occ(union_des_motif) ;


    //Tri décroissant selon le nombre de doublons d'une occurence du tableau contenant l'union, puis affichage.
    tri_decroissant(union_des_motif) ; 
    afficher_tab_occ(union_des_motif) ; 

    //Test du bon fonctionnement de la fonction search_motif par biais de tableau statique et 
    //résultats connus à l'avance. 
    test_fonc_search_motif(lettre_test , _motif_test_1) ; 

    //Test du bon fonctionnement de la fonction union_motif par biais de tableau statique et 
    //résultats connus à l'avance.
    test_fonc_union_motif(lettre_test) ; 

    //Test automatique vérifiant que le tableau est bien dans l'ordre décroissant. 
    test_fonc_tri_decroissant(union_des_motif); 

    //Conversion tableau de motif_t en tableau de int résumant les indices 
    //int * indice_tab = convert_int_tab(mes_mots_motif_1 , mes_mots_motif_2) ; 
    
    //Libération de mémoire 
    //free(indice_tab) ; 
    free(tab_lettre);
    free_tab_occ(mes_mots_motif_1) ; 
    free_tab_occ(mes_mots_motif_2) ; 
    free_tab_occ(union_des_motif);
     
}