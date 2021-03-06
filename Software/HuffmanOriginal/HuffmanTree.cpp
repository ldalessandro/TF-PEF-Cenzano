#include "HuffmanTree.h"

HuffmanTree::HuffmanTree(int n)
{
    tam = n;
    for(int i=0; i<MAXSYMBS; i++)
    {
        signos[i] = ' ';
        codigos[i] = new HuffmanCode();
    }
}

HuffmanTree::~HuffmanTree()
{
    for( int i=0; i<MAXSYMBS; i++ )
    {
        delete codigos[i];
    }
}

void HuffmanTree::setNodo(unsigned char x, int f, int i)
{
    setSigno(x, i);
    setFrecuencia(i, f);
}

void HuffmanTree::setNodo(HuffmanNode hn, int i)
{
    arbol[i].setFrecuencia(hn.getFrecuencia());
    arbol[i].setLeft(hn.isLeft());
    arbol[i].setIzquierdo(hn.getIzquierdo());
    arbol[i].setDerecho(hn.getDerecho());
    arbol[i].setPadre(hn.getPadre());
}

HuffmanCode * HuffmanTree::getCodigo( unsigned char x )
{
    int i = buscar(x);
    return getCodigo(i);
}

void HuffmanTree::setCodigo(HuffmanCode *c, unsigned char x)
{
    int i = buscar( x );
    codigos[i] = c;
}

void HuffmanTree::encode()
{
    makeTree();
    makeCodes();
}

void HuffmanTree::makeTree()
{
    // crear una cola de prioridades ordenada por las frecuencias de los simbolos...
    Heap *cp = new Heap();
    for(int i = 0; i < tam; i++)
    {
        Frequency *fr = new Frequency(i, this->getFrecuencia(i));
        cp->add(fr);
    }

    // p apunta al siguiente nodo libre disponible en el arbol... 
    for(int p = tam; p < 2 * tam - 1; p++)
    {
        // ...tomar los descriptores de frecuencia p1 y p2 con menor frecuencia de la cola de prioridad...
        Frequency *p1 = (Frequency *) cp->remove();
        Frequency *p2 = (Frequency *) cp->remove();

        // ...ajustar ambos nodos como hijos del nuevo nodo p...
        int ip1 = p1->getIndice();
        int ip2 = p2->getIndice();
        arbol[ip1].setPadre(p);
        arbol[ip1].setLeft(true);
        arbol[ip2].setPadre(p);
        arbol[ip2].setLeft(false);

        // calcular la frecuencia del nodo union que se acaba de crear...
        int f = arbol[ip1].getFrecuencia() + arbol[ip2].getFrecuencia();

        // guardar esa frecuencia en el nodo union..
        arbol[p].setFrecuencia(f);

        // guardar los indices de sus hijos...
        arbol[p].setIzquierdo(ip1);
        arbol[p].setDerecho(ip2);

        // agregar el descriptor de frecuencia del nodo union a la cola de prioridad...
        Frequency *nf = new Frequency (p, f);
        cp->add(nf);
    }    
    delete cp;
}

void HuffmanTree::makeCodes()
{
    // indice de la raiz en el arreglo que representa al arbol...
    int iraiz = (2 * tam - 1) - 1;

    // ahora extraemos los codigos del arbol y los guardamos en el vector de codigos
    for(int i=0; i<tam; i++)
    {
        // navegamos hacia arriba...
        int p = i;
        while(p != iraiz)
        {
            if(arbol[p].isLeft())
            {
                codigos[i]->setBit(0);
            }
            else
            {
                codigos[i]->setBit(1);
            }
            p = arbol[p].getPadre();
        }
    }    
}

string HuffmanTree::toString()
{
    string res = "Signos Codificados";

    for(int i=0; i<tam; i++)
    {
        char x[2];
        x[0] = signos[i];
        x[1] = '\0';

        res.append("\nSigno: ");
        res.append(x);
        res.append("\tCodigo: ");
        res.append(codigos[i]->toString());
    }

    return res;
}

// **** metodos privados
int HuffmanTree::buscar(unsigned char x)
{
    for(int i = 0; i<tam; i++)
    {
        if( x == signos[i] ) return i;
    }
    return -1;
}
