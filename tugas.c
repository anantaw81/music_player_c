#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <stdbool.h>

#define max_hash 27

struct n_lagu{
    char nama[50+1];

};

struct utama{
    char nama_pl[50+1];
    struct utama *next;
    struct utama *prev;
    struct daf_putar *bagian[max_hash];
};

struct daf_putar{
    struct n_lagu data;
    //char data[50+1];
    struct daf_putar *next;
    struct daf_putar *prev;
};


//pembuatan data;

struct utama *kepala = NULL;
struct utama *ekor = NULL;

struct utama *playlistnya = NULL; //menandai playlistnya
struct daf_putar *lagunya = NULL;  //menandai lagunya
int nodaf_putar = 0;



struct daf_putar *kepalaa=NULL; //untuk recently_played
//hella ya ini utamas

void init_null_daf_putar(struct utama *init){
    for (int i = 0; i< max_hash; i++){
        init->bagian[i] = NULL;
    }
}


char st[256];
char *masukan_string(char *keterangan, int panjang_maks)
{
    do{
        printf(keterangan);
        fflush(stdin);
        gets(st);
        fflush(stdin);
        if(strlen(st) > panjang_maks){
            printf("Terlalu panjang maksimum %d karakter\n", panjang_maks);
        }
    }while(strlen(st) > panjang_maks);
    return st;
}

int hash_fungsi(char *nama){
    int nilai = (int) nama[0];
    if(nilai < 123 && nilai > 96){
        nilai -= 97;
    }
    else if(nilai < 91 && nilai > 64){
        nilai -= 65;
    }
    else{
        nilai = 26;
    }
    return nilai;
}


int string_compere(char *str1, char *str2, int panjang){  //mengcompare string !. jika str1 > str2 => 1, -1 untuk sama
    int ya = 0;
    int yi = 0;
    int i;
    for(i=0; i<panjang; i++){
        if(str1[i]=='\0' && str2[i]=='\0') break;

        if(str1[i]>str2[i]){
            ya++;
        }
        else if(str1[i]<str2[i]){
            yi++;
        }

        if(ya>yi) break;
        else if(ya<yi) break;

    }

    if(ya>yi) return 1;
    else if(ya<yi) return 0;
    else{
        return -1;
    }
}


//================================
//daftar putar utama

struct utama *cek_data(struct utama **data_cek){
    if (data_cek == NULL)
        return NULL;
    else return data_cek;
};


int tambah_data_utama(char *ket)
{
    struct utama *tambah = (struct utama*)malloc (sizeof(struct utama));
    strcpy(tambah -> nama_pl, ket);
    tambah -> next = NULL;
    tambah -> prev = NULL;
    init_null_daf_putar(tambah); //mengisi daf_putar bagian dgn null semua.


    if(cek_data(kepala) == NULL){
        kepala = tambah;
        ekor = tambah;
    }
    else{
        ekor -> next = tambah;
        tambah -> prev = ekor;
        ekor = tambah;
    }

    return 1;
};

void tampilkan_data_utama(){
    int no = 0;
    if (cek_data(kepala)==NULL){
        puts("** Data Masih Kosong **");
    }
    else if(cek_data(kepala->next)==NULL){
        printf("[%d]  %s\n", no++, kepala->nama_pl);
    }
    else{
        struct utama *bantu = kepala;
        do{
            printf("[%d]  %s\n", no++, bantu->nama_pl);
            bantu = bantu -> next;
        }while(bantu  != NULL);
    }
}

struct utama *cek_data_utama(int nilai_dic){
    int i =0;
    struct utama *bantu = kepala;
    while(i < nilai_dic-1){
        bantu = bantu->next;
        i++;
    }
    return bantu;
};

void hapus_data_utama(int nilai){
    struct utama *help, *bantu;
    help = cek_data_utama(nilai);

    if(help == playlistnya){
        if(playlistnya->next)
            playlistnya=playlistnya->next;
        else if(playlistnya->prev){
            playlistnya=playlistnya->prev;
        }else{playlistnya = NULL;}
    }


    if(help == NULL){
        puts("");
        return 0;
    }
    if(help == kepala && help == ekor){
        kepala = NULL;
        ekor = NULL;
    }
    else if(help == kepala){
        kepala = kepala -> next;
        kepala -> prev = NULL;
        free(help);
    }
    else if(help == ekor){
        ekor = ekor -> prev;
        ekor -> next = NULL;
        free(help);
    }
    else{
        bantu = help -> prev;
        bantu -> next = help -> next;
        (help -> next) -> prev = bantu;
        free(help);
    }

    puts("** Data berhasil dihapus **");
};

void inisialisasi_utama(){
    tambah_data_utama("All Songs");
    tambah_data_utama("Playlist1");
    tambah_data_utama("Playlist2");
}

//daftar putar utama
//======================


void export_data_file_chain(struct utama *over){
    struct n_lagu simpan_file;
    FILE *f_data;
    if((f_data=fopen("DAFTARMUSIC.DAT", "wb"))==NULL){
                puts("File tidak dapat diciptakan");
                }

    struct daf_putar *bantu;

    for (int i =0; i<max_hash; i++){
        bantu = over->bagian[i];
        if(!bantu){
        }
        else{
            do{
                strcpy(&simpan_file.nama, bantu->data.nama);

                fwrite(&simpan_file, sizeof(simpan_file), 1, f_data);

                bantu = bantu->next;


            }while(bantu);
            }
        }


    fclose(f_data);

}

void insert_data_file_chain(struct utama *over){
    struct n_lagu bantu;
    //struct daf_putar bantu;
    struct daf_putar *help;


    FILE *f_data;
    if((f_data=fopen("DAFTARMUSIC.DAT", "rb"))==NULL){
        puts("File tidak dapat dibuka");
    }
    while(fread(&bantu, sizeof(bantu), 1, f_data)==1){
        struct daf_putar *tambah = (struct daf_putar*) malloc(sizeof(struct daf_putar));
        strcpy(tambah->data.nama, &bantu.nama);
        tambah -> next = NULL;
        tambah -> prev = NULL;

        int hash_index = hash_fungsi(bantu.nama);
        help = over -> bagian[hash_index];


        if(!help){
            over -> bagian[hash_index] = tambah;
        }
        else{
            while(help->next){
            help = help-> next;
            }
            help -> next = tambah;
            tambah -> prev = help;
        }

    }
    fclose(f_data);
}


void insert_chain(struct utama *over, char *helo){
    char temp[50];

    struct daf_putar *tambah = (struct daf_putar*) malloc(sizeof(struct daf_putar));
    struct daf_putar *bantu;
    strcpy(tambah->data.nama, helo);
    tambah -> next = NULL;
    tambah -> prev = NULL;



    int hash_index = hash_fungsi(helo);
//  printf("%p", kepala->bagian[hash_index]);
    bantu = over -> bagian[hash_index];
//   puts("hello");
    if(!bantu){  //if bantu == null
        over -> bagian[hash_index] = tambah;
//        puts("hello");
    }
    else{

        while(bantu->next){
            bantu = bantu-> next;
            }
            if(string_compere(tambah->data.nama, bantu->data.nama,50)){
            strcpy(temp, tambah->data.nama);
            strcpy(tambah->data.nama, bantu->data.nama);
            strcpy(bantu->data.nama, temp);
            }
            bantu -> next = tambah;
            tambah -> prev = bantu;
    }
}

void display_chain(struct utama *over){
    struct daf_putar *bantu;
    int j = 1;

    for (int i =0; i<max_hash; i++){
        bantu = over->bagian[i];
        //printf("[%d]", i+1);
        if(!bantu){
            //printf("~~");
        }
        else{
            do{
                printf("[%d] %s \n", j++, bantu->data.nama);
                bantu = bantu->next;
            }while(bantu);
            }
        }
    }


int nullkan_kepala(struct utama *over, char *helo){
    int hash_index = hash_fungsi(helo);
    over->bagian[hash_index] = NULL;
}
int majukan_kepala(struct utama *over, char *helo){
    int hash_index = hash_fungsi(helo);
    (over->bagian[hash_index]) = (over->bagian[hash_index])->next; ;
    (over->bagian[hash_index]) -> prev = NULL;
}


struct daf_putar *search_chain(struct utama *over, char *helo){
    int hash_index = hash_fungsi(helo);
    struct daf_putar *bantu;

    if(over->bagian[hash_index]){
        bantu = over -> bagian[hash_index];
        while(bantu){
        if(strcmp(bantu->data.nama, helo)==0){
            return bantu;
        }
        bantu = bantu->next;
        }
    }
    return NULL;
}

struct daf_putar *hapus_chain(struct utama *over, struct daf_putar *bantu, char *helo){
    struct daf_putar *help;
    if(!bantu) return NULL;

    if(!(bantu->next) && !(bantu->prev)){
        nullkan_kepala(over, helo);
    }
    else if(!bantu->next){ //bantu next null
        help = bantu->prev;
        help -> next = NULL;
        free(bantu);
    }
    else if(!bantu->prev){  //bantu prev null artinya kepala
        majukan_kepala(over, helo);
        free(bantu);
    }
    else{
        help = bantu->prev;
        help -> next = bantu -> next;
        (bantu->next) -> prev = help;
    }
    return help;
}

struct daf_putar *no_lagunya(struct utama *over, int no){
    struct daf_putar *help;
    int i = 0, j=0;
    //help = over->bagian[j];
    while(1){
        help = over->bagian[j];
        while(help){
            if(i==no-1) return help;
            i++;
            help = help->next;
        }
        j++;
    }
    return NULL;
}


void putar_lagu_next(int *i){
        if(lagunya->next){
            lagunya=lagunya->next;
        }
        else{
            do{
                lagunya = playlistnya->bagian[++*i];
            }while(!lagunya);
        }
}

void putar_lagu_prev(int *i){
    if(lagunya->next){
            lagunya=lagunya->next;
        }
    else{
        do{
            lagunya = playlistnya->bagian[--*i];
        }while(!lagunya);
    }
}

void putar_pertama(){
    int i =0;
    lagunya = NULL;
    do{
        lagunya = playlistnya->bagian[i++];
    }while(!lagunya);
    nodaf_putar = i-1;
}

void putar_lagu(){
    int car;
    bool batas = true;
    char icon = 233;
    display_chain(playlistnya);
    putar_pertama();
    do{
    system("cls");
    puts("______________________________");
    printf("Lagu yang sedang diputar : \n");
    printf("\n");
    printf("%s", lagunya->data.nama);
    add_recently(lagunya);
    printf("\n");
    printf("\t%c\t%c\t%c\n", 174, icon, 175);
    puts("______________________________");
    puts("");
    puts("1. Prev ");
    puts("2. Pause ");
    puts("3. Next ");
    puts("4. Kembali ");
    printf("Ketik Pilihan : ");
    scanf("%d", &car);

    switch(car){
        case 1:
            putar_lagu_prev(&nodaf_putar);
            break;
        case 2:
            if(icon == 254){
                icon =233;
            }else{
                icon =254;
            }
            break;
        case 3:
            putar_lagu_next(&nodaf_putar);
            break;
        case 4:
            batas = false;
            break;
        default:
            puts("PILIHAN SALAH");
    }

    }while(batas);

}


void tampilan_utama(){
    struct daf_putar *cari_lagu;
    bool batas = true;
    inisialisasi_utama();
    int pil;
    do{
    puts("Selamat Datang *USER*");
    puts("1. Tambahkan Lagu");
    puts("2. Lihat Seluruh Lagu");
    puts("3. Tambahkan Playlist");
    puts("4. Lihat & Edit Playlist");
    puts("5. Recently Played");
    puts("6. Cari Lagu");
    puts("7. Keluar");
    puts("8. Import Lagu");
    puts("9. Export Lagu");
    printf("Masukkan Pilihan : ");
    scanf("%d", &pil);

    switch(pil){
        case 1:
            playlistnya=kepala;
            insert_chain(playlistnya, masukan_string("Masukkan nama lagu : ",50));
            break;
        case 2:
            playlistnya=kepala;
            display_chain(playlistnya);
            break;
        case 3:
            tambah_data_utama(masukan_string("Masukkan nama playlist : ",50));
            break;
        case 4:
            playlist();
            break;
        case 5:
            puts("RECENTLY PLAYED LIST");
            display_recently();
            break;
        case 6:
            cari_lagu = search_chain(kepala, masukan_string("Masukkan nama lagu yang ingin dicari :", 50));
            if(cari_lagu == NULL){
                printf("Lagu tidak ditemukan !\n");
            }else{
                printf("Lagu [%s] ditemukan !\n", cari_lagu->data.nama);
            }
            break;
        case 7:
            batas = false;
            break;
        case 8:
            playlistnya=kepala;
            puts("Mengimport lagu dari DAFTARMUSIC.DAT");
            insert_data_file_chain(playlistnya);
            break;
        case 9:
            playlistnya=kepala;
            puts("Export lagu ke DAFTARMUSIC.DAT");
            export_data_file_chain(playlistnya);
            break;
        default:
            printf("Inputan Salah !");
            batas = false;

    }
    }while(batas);
}

void change_selected_playlist(int no){
    playlistnya=kepala;
    for(int i=0; i<no; i++){
        playlistnya = playlistnya->next;
    }
}

void playlist(){
    int pil, no, nodata, no_hapus;
    char no_hapus1;
    bool batas = true;
    do{
    system("cls");
    puts("**List Playlist**");
    tampilkan_data_utama();
    puts("");
    puts("Pilihan");
    puts("1. Tambahkan Lagu Ke Playlist No : ");
    puts("2. Lihat (&Hapus) Isi Playlist No : ");
    puts("3. Putar Playlist No :");
    puts("4. Hapus Playlist No :");
    puts("5. Kembali");
    printf("Masukkan pilihan : ");
    scanf("%d", &pil);

    change_selected_playlist(1);

    switch(pil){
        case 1:
            printf("Tambahkan Lagu Ke Playlist No : ");
            scanf("%d", &no);
            change_selected_playlist(no);
            display_chain(kepala);
            printf("Mau input data no berapa ?");
            scanf("%d", &nodata);
            insert_chain(playlistnya, no_lagunya(kepala, nodata)->data.nama);
            break;
        case 2:
            printf("Lihat Isi Playlist No : ");
            scanf("%d", &no);
            change_selected_playlist(no);
            display_chain(playlistnya);
            printf("\n Ingin menghapus lagu ? (Y/N)");
            fflush(stdin);
            scanf("%c", &no_hapus1);
            if(no_hapus1=='Y' || no_hapus1=='y'){
            do{
            printf("\n Ingin menghapus lagu no : ");
            scanf("%d", &no_hapus);
            hapus_chain(playlistnya,no_lagunya(playlistnya,no_hapus),no_lagunya(playlistnya,no_hapus)->data.nama);
            printf("\n Ingin menghapus lagu (lagi) ? (Y/N)");
            fflush(stdin);
            scanf("%c", &no_hapus1);
            }while(no_hapus1=='Y' || no_hapus1=='y');
            }
            break;
        case 3:
            printf("Putar Playlist No : ");
            scanf("%d", &no);
            change_selected_playlist(no);
            putar_lagu();
            break;
        case 4:
            printf("Hapus Playlist No : ");
            scanf("%d", &no);
            hapus_data_utama(no+1);
            break;
        case 5:
            batas = false;
            break;
        default:
            puts("Pilihan SALAH !");
            batas = false;
    }
    puts("");
    }while(batas);

}


void add_recently(struct daf_putar *over){ //ganti pake struct;
    struct daf_putar *tambah = (struct daf_putar*)malloc(sizeof(struct daf_putar));
    strcpy(tambah->data.nama, over->data.nama);
    tambah->next=NULL;
    tambah->prev=NULL;

    if(!kepalaa){
        kepalaa = tambah;
    }
    else{
        kepalaa -> prev = tambah;
        tambah -> next = kepalaa;
        kepalaa = tambah;
    }
}

void display_recently(){
    int i =1;
    struct daf_putar *bantu = kepalaa;
    while(bantu){
        printf("[%d] %s\n", i++, bantu->data.nama);
        bantu = bantu->next;
    }
}

int main(){
    tampilan_utama();
    return 0;
}
