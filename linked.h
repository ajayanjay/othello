#ifndef linked_H
#define linked_H
#define nil NULL

#define info(P) (P)->info
#define next(P) (P)->next

typedef char infotype;
typedef struct tElmtList *address;
typedef struct tElmtList {
    infotype info;
    address next;
} ElmtList;

int isEmpty(address p);
void CreateEmpty(address *q);
void Create_Node(address *p);
void Isi_Node(address *p, infotype nilai);
address SetNode (infotype *nilai);
char InsertTitle ();
void Tampil_List(address p);
void Ins_Awal(address *p, address PNew);
void Ins_Akhir(address *p, address PNew);
address Search(address p, infotype nilai);
void InsertAfter(address *pBef, address PNew);
void Del_Awal(address *p, infotype *X);
void Del_Akhir(address *p, infotype *X);
void DeleteValue (address *p, infotype target, infotype *X);
void DeAlokasi(address *p);
int NbElmt(address p);
void ModifyString(infotype *str);

#endif