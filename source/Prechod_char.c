/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "./../header/messengerDEP.h"
#include "./../header/platformDEP32mk.h"
#include "./../header/pamet.h"
#include <stdbool.h>
#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */
void initPrechChar(PRECH_CHAR *Ptr_PrechCharData){
    Ptr_PrechCharData->periodaVzorkovani =0;
    Ptr_PrechCharData->zetezovatelPrechChar = 0;
    Ptr_PrechCharData->validDataPrechChar = 0;
    unsigned char ctr = 0; //pouze pomocna promenna, kterou vynuluji pole
    for(ctr = 0; ctr <= 100; ctr++){ //vynuluji pole o 101 prvcich
        Ptr_PrechCharData->dataChar[ctr] = 0;
    }
    Ptr_PrechCharData->odesli =0;
    Ptr_PrechCharData->odeslano = 0;
    Ptr_PrechCharData->runPrechChar=0;
}

void runPrechodChar(PRECH_CHAR *Ptr_PrechCharData, CAPTURE_RTM *Ptr_CaptureRTM){
    static unsigned char perioda = 0;
    static unsigned char stav = 0;
    static unsigned char ctr = 1; //chci aby ctr sel az od 1 nebot do 0 prvku zapisuji nejakou random vysokou hodnotu abych vedel odkud prechodovou char merim
    switch(stav){
        case 0: { //disable menice a kontrola zda mam nacteny hodnoty pro periodu a zatezovatel, jdu dal
            setDisableConverter();
            if(Ptr_PrechCharData->validDataPrechChar ==1){
                stav =1;
            }
            break; 
        }
        
        case 1: { //reset menice a jdu dal
            setConverterError(true);
            setConverterUlegErrorFlag(false);
            setConverterVlegErrorFlag(false);
            setConverterError(false);
            stav = 2;
            break;
        }
        
        case 2: { //zapinam menic a jdu dal
            setEnableConverter();
            stav = 3;
            break;
        }
        
        case 3: {
            perioda++; //pricitam s 1ms kdy podle toho jakou si nastavim hodnotu na RTM tak do te chvile pricitam
            if (perioda == Ptr_PrechCharData->periodaVzorkovani){
                if (ctr <= 99){ //pokud mam periodu se kterou vzorkuji, ulozim si hodnotu do pole, jdu do dalsiho stavu, pokud mam 98 tak se pricte na 99
                    Ptr_PrechCharData->dataChar[0] = 1000; //do prvniho prvku pole si dam hodnotu 1000 a pote jiz ctu ty skutecne, abych vedel odkud ctu prechodovou char
                    Ptr_PrechCharData->dataChar[ctr] = Ptr_CaptureRTM->otacky;//hodnoty co do pole ulozim jsou z Capture - ukladam hodnoty od 1
                    perioda = 0;
                    ctr++; //citam do pole, chci zapisovat vzdy s prichodem dalsi periody (ctr inkrementuji az nakonci kdy mam hodnotu zapsanou)
                    stav = 3; //jdu do dalsiho stavu
                }
                if(ctr == 100){ //pokud mam 100 tak zapisu posledni hodnotu a odesilam data, shazuji flagy a jdu do stavu 4
                    Ptr_PrechCharData->dataChar[ctr] = Ptr_CaptureRTM->otacky; //ulozim si 100 prvek do pole (mam pole o 101 prvcich, kdy citam od 0, tzn. muzu citat az do 100)
                    Ptr_PrechCharData->odesli=1; //davam flag ze mohu odesilat do RTM
                    Ptr_PrechCharData->runPrechChar=0; //zastavuji odesilani zatezovatele z RTM z Commandu 3 (Z tohoto commandu nastavuji zatezovatel pro prechodovku)
                    perioda = 0; //nuluji periodu 
                    ctr = 0; //nuluji counter
                    stav = 4; //jdu do dalsiho stavu
                }
            }
            break;
        }
        
        case 4: {
            if (Ptr_PrechCharData->odeslano==1){ //pokud dostanu informaci z RTM ze mam odeslano, tak jdu do stavu 0 a shazuji flagy - ve stavu 0 vypnu menic a cekam na dalsi prikaz
            Ptr_PrechCharData->odeslano=0;
            stav = 0;
            //pridat jeste vynulovani pole
            }
            else{
            stav = 4;
            }
        break;
        }
    
    
    }//konec switche
    
    
    
        
        
        
}
    
    
    
    
    
    
    
    






/* *****************************************************************************
 End of File
 */