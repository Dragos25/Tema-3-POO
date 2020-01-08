#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <typeinfo>

using namespace std;

struct data
{
    int luna;
    int an;
};

class Tranzactie
{
private:
    float valoare;
    string tip;

public:
    Tranzactie()
    {
        valoare=0;
        tip=" ";
    }
    Tranzactie(float v)
    {
        if(v>=0)
            {tip="depun"; valoare=v;}
        else
        {
            tip="retrag";
            valoare=v;
        }

    }

    friend ostream &operator<<(ostream&out, Tranzactie &t)
    {
        out<<t.tip<<":"<<t.valoare;
        return out;
    }

    friend class Cont;
    friend class Cont_economii;
    friend class Cont_curent;

};

class Cont
{
protected:
    string detinator;
    data data_cont;
    float sold;



public:
    Cont()
    {
        detinator=" ";
        sold=0;
        data_cont.luna=0;
        data_cont.an=0;

    }

    Cont(string d, float s, data date)
    {
        detinator=d;
        sold=s;
        data_cont.luna=date.luna;
        data_cont.an=date.an;
    }

    friend istream &operator>>(istream&in, Cont&c)
    {
        cout<<"Nume: ";
        in>>c.detinator;
        cout<<"Data luna an: ";
        in>>c.data_cont.luna;
        in>>c.data_cont.an;
        cout<<"Sold curent: ";
        in>>c.sold;
        return in;
    }

    int actualizare_sold(Tranzactie t)
    {

        if(sold+t.valoare<=0) return -1;
        else sold+=t.valoare;
        return 1;
    }

    virtual void afisare()
    {
        cout<<"Proprietar: "<<detinator<<endl;
        cout<<"Data: "<<data_cont.luna<< "/"<<data_cont.an<<endl;
        cout<<"Sold: "<<sold<<endl;
    }

    bool operator ==(const Cont &c)
    {
        if(detinator==c.detinator)
            return true;
        else
            return false;
    }


};

class Cont_economii:  public Cont
{
private:
    float dobanda;
    int tip_dobanda;
public:
    Cont_economii():Cont()
    {
        dobanda=0;
        tip_dobanda=0;
    }

    Cont_economii(string d, float s, data date,float dob, int tip_dob):Cont(d,s,date)
    {
        dobanda=dob;
        tip_dobanda=tip_dob;
    }

    friend istream &operator>>(istream&in, Cont_economii &c)
    {
        cout<<"Nume: ";
        in>>c.detinator;
        cout<<"Data luna an: ";
        in>>c.data_cont.luna;
        in>>c.data_cont.an;
        cout<<"Sold curent: ";
        in>>c.sold;
        cout<<"Dobanda: ";
        in>>c.dobanda;
        cout<<"Rata dobanzii: ";
        in>>c.tip_dobanda;
        return in;
    }

    void afisare()
    {
        Cont::afisare();
        cout<<"Dobanda: "<<dobanda<<endl;
        cout<<"Tip dobanda: "<<tip_dobanda<<endl;
    }

};

class Cont_curent:public Cont
{
private:
    int gratuit;

public:
    Cont_curent():Cont()
    {
        gratuit=0;

    }
    Cont_curent(string d, float s, data date, int gr):Cont(d,s,date)
    {
        gratuit=gr;
    }


    friend istream &operator>>(istream&in, Cont_curent &c)
    {
        cout<<"Nume: ";
        in>>c.detinator;
        cout<<"Data luna an: ";
        in>>c.data_cont.luna;
        in>>c.data_cont.an;
        cout<<"Sold curent: ";
        in>>c.sold;
        cout<<"Numar de tranzactii gratuite: ";
        in>>c.gratuit;
        return in;
    }

    void afisare()
    {
        Cont::afisare();
        cout<<"Nr de tranzactii gratuite:"<<gratuit<<endl;
    }
};


template <class T>
class GestionareConturi {
public:
    static int id;
protected:
    T* c[100];
    unordered_map <int, vector<Tranzactie>> m;
public:


    GestionareConturi()
    {
        id++;
    }
    GestionareConturi(T con)
    {
        c=con;
    }
    int getid()
    {
        return id;
    }

    void operator +=(T &con)
    {

        cout<<id<<endl;
        c[id]=new T;
        *c[id]=con;
        m.insert(pair<int,vector<Tranzactie>>(id, vector<Tranzactie>()));
        id++;
    }

    /*void adaug_cont(T &con)
    {
        cout<<id<<endl;
        c[id]=new T;
        *c[id]=con;
        id++;
    }*/

    void afisez_conturi()
    {
        for(int i=1;i<id;i++)
            (*c[i])->afisare();
    }

    void adaug_tranzactie(int id_cont, Tranzactie &t)
    {

        if(id_cont>id || id_cont<1) {cout<<"Contul nu exista";return;}
          if(  (*c[id_cont])->actualizare_sold(t)==-1 ){cout<<"Nu se poate realiza tranzactia"<<endl;return;}
        //(*c)[id_cont]->afisare();
        auto it = m.begin();
        for(it;it!=m.end();it++)
            if(it->first==id_cont) break;
        if(it!=m.end())
            it->second.push_back(t);

    }

    void afisez_tranzactii(int id_cont)
    {
        if(id_cont>id || id_cont<1) {cout<<"Contul nu exista";return;}
        auto it = m.begin();
        for(it;it!=m.end();it++)
            if(it->first==id_cont) break;
        if(it!=m.end())
        {
        vector<Tranzactie>::iterator p;
        for(p=it->second.begin();p!=it->second.end();p++)
            cout<<*p<<endl;
        }
    }

    void afisez()
    {
        auto it = m.begin();
//        cout<<id<<endl;
        vector<Tranzactie>::iterator p;
        for(p=it->second.begin();p!=it->second.end();p++)
            cout<<*p;
    }
};

template<class T>
int GestionareConturi<T>::id=0;







int main()
{
    /*data date;
    date.an=2020;
    date.luna=1;
    Cont *p=new Cont("Dragos", 300,date);
    Cont *q=new Cont_economii("Ion",3000,date,0.2,3);
    GestionareConturi<Cont*> g;
    g.adaug_cont(p);
    g.adaug_cont(q);
    cout<<GestionareConturi<Cont>::id;
    //p.afisare();
    g.afisez_conturi();*/
    Tranzactie t(30);
    Tranzactie q(-40);
    Tranzactie p(-2000);
    cout<<t<<endl<<q<<endl<<p<<endl;

    GestionareConturi<Cont*> g;



    cout<<"AAAAAAAAA"<<endl;

    Cont *c[100];
    int nr_conturi;
    cin>>nr_conturi;
    for(int i=0;i<nr_conturi;i++)
    {
        string aleg;
        cout<<"Alegeti tipul contului:"<<endl;
        cout<<"normal"<<endl<<"economii"<<endl<<"curent"<<endl;
        cin>>aleg;
        while(aleg!="normal" && aleg!="economii" && aleg!="curent")
        {
            cout<<"alegere nevalida"<<endl;
            cin>>aleg;
        }
        if(aleg=="normal")
        {
            Cont *ax=new Cont;
            cin>>*ax;
            c[i]=ax;
            g+=ax;

        }

        if(aleg=="economii")
        {
            Cont_economii *ax=new Cont_economii;
            cin>>*ax;
            c[i]=ax;
            g+=c[i];
        }

        if(aleg=="curent")
        {
            Cont_curent *ax=new Cont_curent;
            cin>>*ax;
            c[i]=ax;
            g+=c[i];
        }
    }
    g.afisez_conturi();
    int trz=-1;
    cout<<"Doriti sa efectuati tranzactii?"<<endl<<1<<" DA"<<endl<<2<<" NU"<<endl;
    cin>>trz;
    while(trz!=2)
    {
        cout<<"Pe ce cont doriti sa realizati o tranzactie? ";
        int kont;
        cin>>kont;
        cout<<"Ce valoare doriti sa aibe tranzactia? ";
        int val;
        cin>>val;
        Tranzactie t(val);
        g.adaug_tranzactie(kont,t);
        cout<<"Mai doriti sa realizati si alte tranzactii? "<<endl<<1<<" DA"<<endl<<2<<" NU"<<endl;
        cin>>trz;

    }

    //for(int i=0;i<nr_conturi;i++)
      //  c[i]->afisare();
      g.afisez_conturi();

      cout<<"Tranzactii: "<<GestionareConturi<Cont>::id<<endl;
      for(int i=1;i<=g.getid();i++)
        g.afisez_tranzactii(i);
      cout<<endl;
      //g.afisez_tranzactii(2);
    return 0;
}
