# SoalShiftSISOP20_modul2_D07

- Maisie Chiara Salsabila - 05111840000057
- David Bintang - 05111840000090

## Daftar Isi
1. [Nomor 1](#1)
2. [Nomor 2](#2)
3. [Nomor 3](#3)



<a name="1"></a>
## 1. Crontab
### Soal:
Buatlah program C yang menyerupai crontab untuk menjalankan script bash dengan ketentuan sebagai berikut:
Contoh: ./program \* 34 7 /home/somi/test.sh
Program dengan argumen seperti contoh di atas akan menjalankan script test.sh setiap
detik pada jam 07:34.
```
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

//template daemon dari modul
static void daemonize()
{
  // Variabel untuk menyimpan PID
  pid_t pid, sid;        

  // Menyimpan PID dari Child Process
  pid = fork();     

  /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
  if (pid < 0)
    exit(EXIT_FAILURE);

  /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
  if (pid > 0) 
    exit(EXIT_SUCCESS);

    umask(0);

    sid = setsid();
    if (sid < 0) 
      exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}


//b. program mengeluarkan pesan eror jika argumen yang diberikan tidak sesuai
void printError ()
{
    printf ("ERROR, Invalid Argument\n");
}


//main function
int main (int argc, char *argv[])
{
    //c. program berjalan di daemon
    daemonize();

    time_t t = time(NULL);
    struct tm currentTime = *localtime(&t);
    int detik, menit, jam;
    //-1 for anytime
    detik = -1; 
    menit = -1;
    jam = -1;

    if (argc != 5) {
        printError ();
        exit(EXIT_FAILURE);
    }

    //a. 
    //mengganti argumen yang tadinya string menjadi int
    if (argv[1][0] != '*') 
        detik = atoi(argv[1]); 
    if (argv[2][0] != '*') 
        menit = atoi(argv[2]);
    if (argv[3][0] != '*') 
        jam = atoi(argv[3]);
    
    if (jam < -1 || jam > 23 || menit < -1 || menit > 59 || detik < -1 || detik > 59) {
        printError ();
        exit(EXIT_FAILURE);
    }

    while (1) { //crontab jd nerus jalan
        t = time(NULL); //biar update timenya sesuai current time
        struct tm currentTime = *localtime(&t);
        //bakal jalan kalo jamnya memenuhi syarat sesuai current time
        //* berarti argumen yg masuk itu true.
        if ((currentTime.tm_hour == jam || jam == -1) && (currentTime.tm_min == menit || menit == -1) && (currentTime.tm_sec == detik || detik == -1)) { //kalo * == true atau sudah memenuhi current time
            //tidak menggunakan system()
            //execl bikin program mati, jd pake fork == 0 yaitu bikin child, sehingga execl ini jd child
            if (fork()==0) execl("/bin/bash", "bash", argv[argc-1], NULL); 
        }
        sleep(1); //delay 1 detik  
    }
}
```

#### a) Program menerima 4 argumen berupa: i. Detik: 0-59 atau * (any value), ii. Menit: 0-59 atau * (any value), iii. Jam: 0-23 atau * (any value), iv. Path file .sh

#### b) Program akan mengeluarkan pesan error jika argumen yang diberikan tidak sesuai
```
void printError ()
{
    printf ("ERROR, Invalid Argument\n");
}
```

#### c) Program hanya menerima 1 config cron

#### d) Program berjalan di background (daemon)
```
static void daemonize()
{
  // Variabel untuk menyimpan PID
  pid_t pid, sid;        

  // Menyimpan PID dari Child Process
  pid = fork();     

  /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
  if (pid < 0)
    exit(EXIT_FAILURE);

  /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
  if (pid > 0) 
    exit(EXIT_SUCCESS);

    umask(0);

    sid = setsid();
    if (sid < 0) 
      exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}
```
template yang ada di modul 2

#### e) Tidak boleh menggunakan fungsi system()
```
if (fork()==0) execl("/bin/bash", "bash", argv[argc-1], NULL); 
```
- ```execl```
- ```fork``` adalah fungsi system call di C untuk melakukan spawning process. Setelah memanggil fungsi itu, akan terdapat proses baru yang merupakan child process dan mengembalikan nilai 0 untuk child process dan nilai PID untuk parent process.




<a name="2"></a>
## 2. Mimpi Kiwa
### Soal:
Shisoppu mantappu! itulah yang selalu dikatakan Kiwa setiap hari karena sekarang dia merasa sudah jago materi sisop. Karena merasa jago, suatu hari Kiwa iseng membuat sebuah program. Kiwa lalu terbangun dan sedih karena menyadari programnya hanya sebuah mimpi. Buatlah program dalam mimpi Kiwa jadi nyata!
Catatan:
- Tidak boleh memakai system().
- Program utama harus ter-detach dari terminal
Hint:
- Gunakan fitur picsum.photos untuk mendapatkan gambar dengan ukuran
tertentu
- Epoch Unix bisa didapatkan dari time()

#### a) Pertama-tama, Kiwa membuat sebuah folder khusus, di dalamnya dia membuat sebuah program C yang per 30 detik membuat sebuah folder dengan nama timestamp [YYYY-mm-dd_HH:ii:ss].

#### b) Tiap-tiap folder lalu diisi dengan 20 gambar yang di download dari https://picsum.photos/, dimana tiap gambar di download setiap 5 detik. Tiap gambar berbentuk persegi dengan ukuran (t%1000)+100 piksel dimana t adalah detik Epoch Unix. Gambar tersebut diberi nama dengan format timestamp [YYYY-mm-dd_HH:ii:ss].

#### c) Agar rapi, setelah sebuah folder telah terisi oleh 20 gambar, folder akan di zip dan folder akan di delete(sehingga hanya menyisakan .zip).

#### d) Karena takut program tersebut lepas kendali, Kiwa ingin program tersebut men-generate sebuah program "killer" yang siap di run(executable) untuk menterminasi semua operasi program tersebut. Setelah di run, program yang menterminasi ini lalu akan mendelete dirinya sendiri.

#### e) Kiwa menambahkan bahwa program utama bisa dirun dalam dua mode, yaitu MODE_A dan MODE_B. untuk mengaktifkan MODE_A, program harus dijalankan dengan argumen -a. Untuk MODE_B, program harus dijalankan dengan argumen-b. Ketika dijalankan dalam MODE_A, program utama akan langsung menghentikan semua operasinya ketika program killer dijalankan. Untuk MODE_B, ketika program killer dijalankan, program utama akan berhenti tapi membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua folder terisi gambar, terzip lalu di delete).




<a name="3"></a>
## 3. Multiprocessing
### Soal: 
Jaya adalah seorang programmer handal mahasiswa informatika. Suatu hari dia memperoleh tugas yang banyak dan berbeda tetapi harus dikerjakan secara bersamaan (multiprocessing). Karena Jaya terlalu banyak tugas dia jadi stress, jadi bantulah Jaya agar bisa membuat program tersebut.
Catatan :
- Tidak boleh memakai system().
- Tidak boleh memakai function C mkdir() ataupun rename().
- Gunakan exec dan fork
- Direktori “.” dan “..” tidak termasuk
```
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>


int main () 
{
    pid_t child_id1;
    child_id1 = fork ();
    int status;

    pid_t child_id2;
    child_id2 = fork ();

    pid_t child_unzip;
    child_unzip = fork ();

    pid_t child_type;
    child_type = fork ();

    int j = 0;
    int k = 0;

    char filename[256][256];
    char filenamed[256][256];


    if (child_id1 < 0) {
        exit (EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    // A.
    if (child_id1 == 0) {
        if (child_id2 < 0) {
            exit (EXIT_FAILURE);
        }
        if (child_id2 == 0) {
            // this is child
            char *argv[] = {"mkdir", "-p", "modul2/indomie", NULL};
            execv("/bin/mkdir", argv);
        }
        else {
            // this is parent
            while ((wait(&status)) > 0);
            char *argv[] = {"mkdir", "-p", "modul2/sedaap", NULL};
            execv("/bin/mkdir", argv);
        }
    }
    
    else {
        while ((wait(&status)) > 0);

        if (child_unzip == 0) {
            // B. unzip
            char *argv[] = {"unzip", "/home/maisie/modul2/jpg.zip", NULL};
            execv("/usr/bin/unzip", argv);
        }
        else {
            //C. unzip to folder jpg
            while ((wait(&status)) > 0);
            if (child_type == 0) {
                pid_t kelompok, move;
                int nstatus;
                DIR *d;
                struct dirent *dir;
                if ((d = opendir("modul2/jpg")) != NULL) {
                    while ((dir = readdir(d)) != NULL) {
                        if (dir->d_type == DT_REG) {
                            strcpy (filename[j], dir->d_name); //for file
                            j++;
                        }
                        else {
                            strcpy (filenamed[k], dir->d_name); //for dir
                            k++;
                        }
                    }
                    closedir (d);
                }
                //C. sedaap
                for (int i=0; i<j; i++) {
                    kelompok = fork();
                    char name[25];
                    sprintf (name, "modul2/jpg/%s", filename[i]);
                    if (kelompok == 0) {
                        char *argv[] = {"mv", name, "/home/maisie/modul2/sedaap/", NULL};
                        execv("/bin/mv", argv);
                    }
                }
                //c. indomie
                for (int i=2; i<k; i++) {
                    kelompok = fork();
                    char name[25];
                    sprintf (name, "modul2/jpg/%s", filenamed[i]);
                    if (kelompok == 0) {
                        char *argv[] = {"mv", name, "/home/maisie/modul2/indomie/", NULL};
                        execv("/bin/mv", argv);
                    }
                    else {
                        
						}
					}
                }
            }
        }
    }
    return 0;
}
```

#### a) Program buatan jaya harus bisa membuat dua direktori di “/home/[USER]/modul2/”. Direktori yang pertama diberi nama “indomie”, lalu lima detik kemudian membuat direktori yang kedua bernama “sedaap”.
```
if (child_id1 == 0) {
        if (child_id2 < 0) {
            exit (EXIT_FAILURE);
        }
        if (child_id2 == 0) {
            // this is child
            char *argv[] = {"mkdir", "-p", "modul2/indomie", NULL};
            execv("/bin/mkdir", argv);
        }
        else {
            // this is parent
            while ((wait(&status)) > 0);
            char *argv[] = {"mkdir", "-p", "modul2/sedaap", NULL};
            execv("/bin/mkdir", argv);
        }
    }
```

#### b) Kemudian program tersebut harus meng-ekstrak file jpg.zip di direktori “/home/[USER]/modul2/”. Setelah tugas sebelumnya selesai, ternyata tidak hanya itu tugasnya.
```
if (child_unzip == 0) {
            char *argv[] = {"unzip", "/home/maisie/modul2/jpg.zip", NULL};
            execv("/usr/bin/unzip", argv);
}
```

#### c) Diberilah tugas baru yaitu setelah di ekstrak, hasil dari ekstrakan tersebut (di dalam direktori “home/[USER]/modul2/jpg/”) harus dipindahkan sesuai dengan pengelompokan, semua file harus dipindahkan ke “/home/[USER]/modul2/sedaap/” dan semua direktori harus dipindahkan ke “/home/[USER]/modul2/indomie/”.
## Belum berhasil
```
while ((wait(&status)) > 0);
            if (child_type == 0) {
                pid_t kelompok, move;
                int nstatus;
                DIR *d;
                struct dirent *dir;
                if ((d = opendir("modul2/jpg")) != NULL) {
                    while ((dir = readdir(d)) != NULL) {
                        if (dir->d_type == DT_REG) {
                            strcpy (filename[j], dir->d_name); //for file
                            j++;
                        }
                        else {
                            strcpy (filenamed[k], dir->d_name); //for dir
                            k++;
                        }
                    }
                    closedir (d);
                }
                //C. sedaap
                for (int i=0; i<j; i++) {
                    kelompok = fork();
                    char name[25];
                    sprintf (name, "modul2/jpg/%s", filename[i]);
                    if (kelompok == 0) {
                        char *argv[] = {"mv", name, "/home/maisie/modul2/sedaap/", NULL};
                        execv("/bin/mv", argv);
                    }
                }
                //c. indomie
                for (int i=2; i<k; i++) {
                    kelompok = fork();
                    char name[25];
                    sprintf (name, "modul2/jpg/%s", filenamed[i]);
                    if (kelompok == 0) {
                        char *argv[] = {"mv", name, "/home/maisie/modul2/indomie/", NULL};
                        execv("/bin/mv", argv);
                    }
                    else {
                        
						}
					}
    }
}
```

#### d) Untuk setiap direktori yang dipindahkan ke “/home/[USER]/modul2/indomie/” harus membuat dua file kosong. File yang pertama diberi nama “coba1.txt”, lalu 3 detik kemudian membuat file bernama “coba2.txt”. (contoh : “/home/[USER]/modul2/indomie/{nama_folder}/coba1.txt”).
