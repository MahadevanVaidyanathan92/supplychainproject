#include<iostream>
#include<vector>
#include<fstream>
#include<queue>
#include<cmath>
#include<algorithm>
#include<numeric>
using namespace std;
int L=0;
struct firm {
    vector<double> Demand;
    vector<double> OHI;
    vector<double> IP;
    vector<double> Backorder;
    vector<double> Order;
    vector<double> Forecast;
    queue<double> Ord_Not_Rec;
};
double queue_sum(queue<double> &Ord_Not_Rec){
    queue<double>dummy;
    double sum=0, x=0;
    while( !Ord_Not_Rec.empty())
    {
        sum+=Ord_Not_Rec.front();
        x=Ord_Not_Rec.front();
        Ord_Not_Rec.pop();
        dummy.push(x);   

    }
    while(!dummy.empty())
    {
        x=dummy.front();
        dummy.pop();
        Ord_Not_Rec.push(x);
    
    }
    return sum;
}
void pushpop(queue<double> &Q,int O){
    int i=0;
    Q.push(O);
    Q.pop();
}
double stddev(vector<double> V){
    double sum=accumulate(V.begin(),V.end(),0.0);
    double mean=sum/(V.size());
    double MSE;
    int i;
    while(i<=V.size()-1){
        MSE+=(V[i]-mean)*(V[i]-mean);
    }
    MSE=MSE/V.size();
    return MSE;    
}
double ES1(vector<double> D,vector<double> IP, double alpha, vector<double> &F){
    double order=0;
    int i=0;
    // Generate forcast here.
    F.push_back(((1-alpha)*F[F.size()-1])+(alpha*D[D.size()-1]));// forecast at t+1 generated, and stored because passed by ref.
    order=max((L+1)*F[size()-1]-IP[IP.size()-1], order);
    return order;//Self explanatory!!
}

// Function for pushing everytime an order is placed should go here
int main(){
    int i=0;
    int j=0;
    struct firm M,D,W,R;//struct for each firm
    fstream fM,fD,fW,fR;//Declared file objects for each firm.
    fM.open("manufacturer.txt",ios::app);
    fD.open("distributor.txt",ios::app);
    fW.open("wholesaler.txt",ios::app);
    fR.open("retailer.txt",ios::app);

    //Initializations for each firm here.
    M.IP.push_back(10+10*L);//IP at time zero.
    D.IP.push_back(10+10*L);
    W.IP.push_back(10+10*L);
    R.IP.push_back(10+10*L);
    M.OHI.push_back(10);//Inventory at time zero
    D.OHI.push_back(10);
    W.OHI.push_back(10);
    R.OHI.push_back(10);
    M.Order.push_back(10);//Order at time zero
    D.Order.push_back(10);
    W.Order.push_back(10);
    R.Order.push_back(10);//Need to push into queue until L.
    M.Forecast.push_back(10);//Initial Forecast at time zero
    D.Forecast.push_back(10);
    W.Forecast.push_back(10);
    R.Forecast.push_back(10);
    M.Forecast.push_back(10);//Initial Forecast at time one
    D.Forecast.push_back(10);
    W.Forecast.push_back(10);
    R.Forecast.push_back(10);
    R.Demand.push_back(10);
    R.Demand.push_back(10);//D(1) initialized at 10, consistend with end consumer order of 10 at time zero.
    i=0;
    while(i<=L-1 && L!=0){
        M.Order.push_back(10);//Orders coming after zeroth time
        D.Order.push_back(10);
        W.Order.push_back(10);
        R.Order.push_back(10);
         
        i++;
    }
    //Initializations end.






    /*/Testing block start
    queue <double> Q;
    Q.push(9);
    Q.push(19);
    Q.push(21);
    cout<<queue_sum(Q)<<endl;
    cout<<Q.front();
    Q.pop();
    cout<<Q.front();
    Q.pop();
    cout<<Q.front();
    Q.pop();
    *///Testing block end

    return 0;
}

