#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>

void init_mouse();
void show_mouse();
void get_mouse_status();
int bakiye_oku();
void bakiye_guncelle(int yeni_bakiye);
void islem_kaydet(char *alici, int miktar, char *tarih);
void para_gonder();
void bakiye_goster();
void islemleri_goster();
void anamenu_grafik();
void hatali_giris_uyarisi();
int sifre_giris_grafik();
void hesap_bakiyesi(int mevcut);

int bakiye = 50000;
int sifre = 1234;

int mouse_x = 0, mouse_y = 0, mouse_click = 0;

void init_mouse() {
    union REGS r;
    r.x.ax = 0;
    int86(0x33, &r, &r);
}

void show_mouse() {
    union REGS r;
    r.x.ax = 1;
    int86(0x33, &r, &r);
}

void get_mouse_status() {
    union REGS r;
    r.x.ax = 3;
    int86(0x33, &r, &r);
    mouse_click = r.x.bx;
    mouse_x = r.x.cx;
    mouse_y = r.x.dx; 
}

int bakiye_oku() {
    int okunan_bakiye = bakiye;
    FILE *dosya = fopen("bakiye.txt", "r");
    if (dosya != NULL) {
        if (fscanf(dosya, "%d", &okunan_bakiye) != 1) {
            okunan_bakiye = bakiye; 
        }
        fclose(dosya);
    }
    return okunan_bakiye;
}

void bakiye_guncelle(int yeni_bakiye) {
    FILE *dosya = fopen("bakiye.txt", "w");
    if (dosya != NULL) {
        fprintf(dosya, "%d", yeni_bakiye);
        fclose(dosya);
    }
}

void islem_kaydet(char *alici, int miktar, char *tarih) {
    FILE *dosya = fopen("islemler.txt", "a");
    if (dosya != NULL) {
        fprintf(dosya, "Tarih: %s | Alici: %s | Miktar: %d TL\n", tarih, alici, miktar);
        fclose(dosya);
    }
}

void para_gonder() {
    char alici[100], tarih[20];
    int miktar;
    int mevcut = bakiye_oku();

    printf("Alici Adi: ");
    scanf("%s", alici);

    printf("Gonderilecek Miktar: ");
    scanf("%d", &miktar);

    if (miktar <= 0 || miktar > mevcut) {
        printf("Yetersiz veya gecersiz miktar!\n");
        getch();
        return;
    }

    printf("Tarih (gg.aa.yyyy): ");
    scanf("%s", tarih);

    mevcut -= miktar;
    bakiye_guncelle(mevcut);
    islem_kaydet(alici, miktar, tarih);

    printf("Islem basarili! Yeni bakiye: %d TL\n", mevcut);
    getch();
}

void bakiye_goster() {
    int mevcut = bakiye_oku();
    hesap_bakiyesi(mevcut);
}

void islemleri_goster() {
    FILE *dosya = fopen("islemler.txt", "r");
    char satir[200];
    
    if (dosya == NULL) {
        printf("Islem kaydi bulunamadi.\n");
        getch();
        return;
    }
    
    printf("\n--- Yapilan Islemler ---\n");
    while (fgets(satir, sizeof(satir), dosya) != NULL) {
        printf("%s", satir);
    }
    fclose(dosya);
    getch();
}

void anamenu_grafik() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    setbkcolor(BLUE);
    cleardevice();
    setcolor(WHITE);
    rectangle(10, 10, 630, 470);

    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);
    outtextxy(250, 20, "PUTBANK");
    line(230, 23, 410, 23);
    line(230, 57, 410, 57);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
    outtextxy(295, 58, "Mobil");

    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);

    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    bar(250, 115, 390, 145);
    setcolor(BLACK);
    outtextxy(260, 117, "Hesap Bakiyesi");

    bar(250, 160, 390, 190);
    setcolor(BLACK);
    outtextxy(270, 163, "Para Gonder");

    bar(250, 205, 390, 235);
    setcolor(BLACK);
    outtextxy(272, 207, "Son Islemler");

    bar(250, 250, 390, 280);
    setcolor(BLACK);
    outtextxy(300, 254, "Cikis");

    setcolor(WHITE);
    settextstyle(SMALL_FONT, HORIZ_DIR, 5);
    outtextxy(170, 430, "\"Paranizi Guvenebileceginiz En Son Yer\"");

    init_mouse();
    show_mouse();

    while (1) {
        get_mouse_status();
        if (mouse_click == 1) {
            if (mouse_x >= 250 && mouse_x <= 390 && mouse_y >= 115 && mouse_y <= 145) {
                closegraph();
                bakiye_goster();
                anamenu_grafik();
                break;
            }
            if (mouse_x >= 250 && mouse_x <= 390 && mouse_y >= 160 && mouse_y <= 190) {
                closegraph();
                para_gonder();
                anamenu_grafik(); 
                break;
            }
            if (mouse_x >= 250 && mouse_x <= 390 && mouse_y >= 205 && mouse_y <= 235) {
                closegraph();
                islemleri_goster();
                anamenu_grafik();
                break;
            }
            if (mouse_x >= 250 && mouse_x <= 390 && mouse_y >= 250 && mouse_y <= 280) {
                closegraph();
                exit(0);
            }
        }
        delay(50);
    }
}

void hatali_giris_uyarisi() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
    
    setbkcolor(BLUE);
    cleardevice();
    setcolor(WHITE);
    rectangle(10, 10, 630, 470);
    
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);
    outtextxy(250, 20, "PUTBANK");
    line(230, 23, 410, 23);
    line(230, 57, 410, 57);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
    outtextxy(280, 58, "Mobil");
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
    outtextxy(218, 90, "Merhaba Hosgeldiniz");
    
    setcolor(GREEN);
    rectangle(50, 150, 590, 400);
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    bar(60, 160, 580, 390);
    
    setcolor(RED);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 5);
    outtextxy(250, 180, "UYARI!");
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
    outtextxy(100, 250, "3 Kez Ust Uste Hatali Deneme Yaptiniz");
    outtextxy(200, 320, "Uygulamadan Cikiliyor...");
    
    setcolor(WHITE);
    settextstyle(SMALL_FONT, HORIZ_DIR, 5);
    outtextxy(170, 430, "\"Paranizi Guvenebileceginiz En Son Yer\"");
    
    delay(3000);
    closegraph();
}

int sifre_giris_grafik() {
    int giris, deneme_hak = 3;
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
    
    init_mouse();
    show_mouse();
    
    setbkcolor(BLUE);
    cleardevice();
    setcolor(WHITE);
    rectangle(10, 10, 630, 470);
    
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);
    outtextxy(250, 20, "PUTBANK");
    line(230, 23, 410, 23);
    line(230, 57, 410, 57);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
    outtextxy(280, 58, "Mobil");
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
    outtextxy(218, 90, "Merhaba Hosgeldiniz");
    
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 1);
    outtextxy(175, 150, "Giris Yapmak Icin Sifre Giriniz");
    
    setcolor(GREEN);
    rectangle(265, 350, 370, 370);
    setfillstyle(SOLID_FILL, WHITE);
    bar(270, 355, 365, 365);
    
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
    outtextxy(295, 310, "Sifre");
    
    settextstyle(SMALL_FONT, HORIZ_DIR, 5);
    outtextxy(170, 430, "\"Paranizi Guvenebileceginiz En Son Yer\"");
    
    while (1) {
        get_mouse_status();
        if (mouse_click == 1) {
            if (mouse_x >= 270 && mouse_x <= 365 && mouse_y >= 355 && mouse_y <= 365) {
                closegraph();
                while (deneme_hak > 0) {
                    printf("Sifre: ");
                    scanf("%d", &giris);
                    if (giris == sifre) {
                        printf("Giris basarili.\n");
                        delay(1000);
                        return 1;
                    } else {
                        deneme_hak--;
                        printf("Yanlis sifre! Kalan hak: %d\n", deneme_hak);
                    }
                }
                hatali_giris_uyarisi();
                return 0;
            }
        }
        delay(50);
    }
}

void hesap_bakiyesi(int mevcut) {
    int gd = DETECT, gm;
    char bakiye_metni[50];
    
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    setbkcolor(BLUE);
    cleardevice();
    setcolor(WHITE);
    rectangle(10, 10, 630, 470);

    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);
    outtextxy(250, 20, "PUTBANK");
    line(230, 23, 410, 23);
    line(230, 57, 410, 57);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
    outtextxy(295, 58, "Mobil");

    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    bar(250, 115, 390, 145);
    setcolor(BLACK);
    outtextxy(260, 117, "Hesap Bakiyesi");

    setcolor(GREEN);
    rectangle(50, 150, 590, 400);
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    bar(60, 160, 580, 390);

    sprintf(bakiye_metni, "Mevcut Bakiye: %d TL", mevcut);
    setcolor(BLACK);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
    outtextxy(100, 250, bakiye_metni);

    setcolor(WHITE);
    settextstyle(SMALL_FONT, HORIZ_DIR, 5);
    outtextxy(170, 430, "\"Paranizi Guvenebileceginiz En Son Yer\"");
    
    getch();
    closegraph();
}

int main() {
    FILE *dosya = fopen("bakiye.txt", "r");
    int okunan_bakiye;

    if (dosya == NULL || fscanf(dosya, "%d", &okunan_bakiye) != 1) {
        bakiye_guncelle(50000);
    } else {
        fclose(dosya);
    }

    if (sifre_giris_grafik()) {
        anamenu_grafik();
    } else {
        exit(0);
    }

    return 0;
}

