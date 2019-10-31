#include "compac.h"

void contar(FILE *arq)
{
    int c, i = 0;
    huffNode** vetAux = (huffNode**)malloc(sizeof(huffNode)*256);
    for(i = 0; i < 256; i++)
        vetAux[i] = criarHuffNode(666, 1);

    while((c = fgetc(arq)) != EOF)
    {
        /*pode ter quebras de linha, tabulações, carriage return etc*/
        if(c != '\r' && c != '\n' && c != '\t' && c != '\v' && c != '\f')
        {
            if(vetAux[c] -> valueHuffNode == 666)
                vetAux[c] -> valueHuffNode = c;
            else
                vetAux[c] -> frequency += 1;
        }
    }
    rewind(arq);
    /*cria a fila de prioridade de acordo com o metodo de huffman*/
    priQueue = criarFila();
    for(i = 0; i < 256; i++)
        if(vetAux[i] -> valueHuffNode != 666)
            inserir(priQueue, vetAux[i]);
    free(vetAux);
}

void transformBytes(nodeBit** vetBits, FILE *saida, FILE *arq)
{
    int i = 0, indMenor = 0, c = 0, indiceByte = 0;
    char oitoAtual[9] = {0};
    unsigned char byteResultado = 0;
    boolean acabou = false;
    while(!acabou)
    {
        if(vetBits[c] -> value == 666)
        {
            c = fgetc(arq);
            if(c == EOF)
                acabou = true;
        }
        else
        {
            for(; i < 8 && indMenor < strlen(vetBits[c] -> code); i++, indMenor++)
                oitoAtual[i] = vetBits[c] -> code[indMenor];
            if(i < 8)
            {
                indMenor = 0;
                c = fgetc(arq);
                if(c == EOF)
                    acabou = true;
            }
            else if (i == 8)
            {
                i = 0;
                indiceByte = 0;
                for(; indiceByte < 8; indiceByte++)
                    if(oitoAtual[indiceByte] == '1')            /**1 = 00000001*/
                        byteResultado |= 1 << (7 - indiceByte); /**coloca 1 na posicao (7 -indiceByte) e mescla com o anterior*/
                for(; i < 9; i++)
                        oitoAtual[i] = 0;
                i = 0;
                fputc(byteResultado, saida);
                byteResultado = 0;
                if(indMenor == strlen(vetBits[c] -> code)) /**percorreu todo o cod atual*/
                {
                    indMenor = 0;
                    c = fgetc(arq);
                    if(c == EOF)
                        acabou = true;
                }
            }
        }
    }
    if (i < 8)
    {
        indiceByte = 0;
        for(; indiceByte < 8; indiceByte++)
            if(oitoAtual[indiceByte] == '1')
                byteResultado |= 1 << (7 - indiceByte);
        fputc(byteResultado, saida);
        byteResultado = 0;
    }
}

void compactar(FILE *arq, char dir[])
{
    huffNode* huffTree = criarArvore(priQueue);
    int i = 0, c = 0, qtdBits = 0;
    unsigned char bitsLixo = 0;
    FILE *saida = fopen("C:/Users/u18196/Desktop/t.joojar", "wb");
    nodeBit** vetBits = (nodeBit**)malloc(sizeof(nodeBit*)*256);

    for(i = 0; i < 256; i++)
        vetBits[i] = criarNodeBit(666,"a");
    char* vetCode = (char*)malloc(sizeof(char)*alturaArvore(huffTree));
    for(i = 0; i < alturaArvore(huffTree); i++)
        vetCode[i] = 0;
    transformarEmBits(huffTree, vetCode, vetBits);

    while((c = fgetc(arq)) != EOF)
        qtdBits += strlen(vetBits[c] -> code);
    bitsLixo = qtdBits % 8;
    if(bitsLixo != 0)
        fputc(bitsLixo, saida);
    rewind(arq);
    printarArq(huffTree, saida);
    transformBytes(vetBits, saida, arq);

    free(huffTree);
    free(vetCode);
    free(vetBits);
    free(priQueue);
}

void descompactar(FILE *arq)
{

}
