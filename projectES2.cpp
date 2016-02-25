#include<iostream>
#include<vector>
#include<fstream>
#include<queue>
#include<cmath>
#include<algorithm>
#include<numeric>
using namespace std;
int L=0;
double ES23=0.803;
vector<double>demands;
struct firm {
    vector<double> Demand;
    vector<double> OHI;
    vector<double> IP;
    vector<double> Backorder;
    vector<double> Order;
    vector<double> Forecast;
    vector<double> Received;
    vector<double> Sent;
    vector<double> Totalinv;
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
void pushpop(queue<double> &Q,double O){
    int i=0;
    Q.push(O);
    Q.pop();
}
double stddev(vector<double> V){
        
    double sum=0;//=accumulate(V.begin(),V.end(),0);
    int j =1000;
    while(j<V.size()){
        sum+=V[j];
        j++;
    }
    double mean=sum/(V.size()-1000);
    double MSE=0;
    int i=1000;
    while(i<=V.size()-1){
        MSE+=(V[i]-mean)*(V[i]-mean);
        i++;
    }
    //cout<<"Size of the Vector = "<<V.size()<< endl;
    MSE=MSE/(V.size()-1001);
    MSE=pow(MSE,0.5);
    return MSE;    
}
double ES1(vector<double> &D,vector<double> &IP, double alpha, vector<double> &F,int k){
    double order=0;
    int i=0;
    // Generate forcast here.
    F.push_back(((1-alpha)*F[k+1])+(alpha*D[k+1]));// forecast at t+1 generated, and stored because passed by ref.
    if((L+1)*(F[F.size()-1])-(IP[k+1])>=0)
        order=((L+1)*F[k+2]-IP[k+1]);
    else{
        order=0;}
    return order;//Self explanatory!!
}
double ES12(vector<double> &D,vector<double> &IP, double alpha, vector<double> &F,int k){
    double order=0;
    int i=0;
    // Generate forcast here.
    F.push_back(((1-alpha)*F[k+1])+(alpha*D[k+1]));// forecast at t+1 generated, and stored because passed by ref.
    if((L+1)*(F[k+2])-(IP[k+1])>=0)
        order=((L+1)*F[k+2]-IP[k+1]);
    else{
        order=0;}
    return order;//Self explanatory!!
}
double ES2(vector<double> &D,vector<double> &IP, double alpha, vector<double> &F,int k){
    double order=0;
    int i=0;
    // Generate forcast here.
    F.push_back(((1-alpha)*F[k+1])+(alpha*D[k+1]));// forecast at t+1 generated, and stored because passed by ref.
    double mean=0;
    while(i<=D.size()-1){
        mean+=D[i];
        i++;}
    mean/=D.size();
    double dev=0;
    if(k==0)dev=0;
    else{
        i=0;
    while(i++<=D.size()-1){
        dev+=(D[i]-mean)*(D[i]-mean);    
    }    
    dev/=k;
    }
    if((L+1)*(F[F.size()-1])-(IP[k+1])+(0.801*(pow((L+1)*dev,0.5)))>=0)
        order=(L+1)*(F[F.size()-1])-(IP[k+1])+(0.801*(pow((L+1)*dev,0.5)));
    else{
        order=0;}
    return order;//Self explanatory!!
}
double ES22(vector<double> &D,vector<double> &IP, double alpha, vector<double> &F,int k){
    double order=0;
    int i=0;
    // Generate forcast here.
    F.push_back(((1-alpha)*F[k+1])+(alpha*D[k+1]));// forecast at t+1 generated, and stored because passed by ref.
double mean=0;
    while(i<=D.size()-1){
        mean+=D[i];
        i++;}
    mean/=D.size();
    double dev=0;
    if(k==0)dev=0;
    else{
        i=0;
    while(i++<=D.size()-1){
        dev+=(D[i]-mean)*(D[i]-mean);    
    }    
    dev/=k;
    }
    if((L+1)*(F[k+2])-(IP[k+1])+(0.801*(pow((L+1)*dev,0.5)))>=0)
        order=(L+1)*(F[k+2])-(IP[k+1])+(0.801*(pow((L+1)*dev,0.5)));
    else{
        order=0;}
    return order;//Self explanatory!!
}
// Function for pushing everytime an order is placed should go here
int main(){
    int i=0;
    int j=0;
    double alpha=0.0;
    struct firm M,D,W,R;//struct for each firm
    fstream fM,fD,fW,fR;//Declared file objects for each firm.
    fM.open("manufacturer.txt",ios::app);
    fD.open("distributor.txt",ios::app);
    fW.open("wholesaler.txt",ios::app);
    fR.open("retailer.txt",ios::app);
    while(alpha<=1.0){
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
    M.Forecast.push_back(0);//Initial Forecast at time zero
    D.Forecast.push_back(0);
    W.Forecast.push_back(0);
    R.Forecast.push_back(0);
    M.Forecast.push_back(10);//Initial Forecast at time one
    D.Forecast.push_back(10);
    W.Forecast.push_back(10);
    R.Forecast.push_back(10);
    M.Backorder.push_back(0);//Initial Backorder at time zero
    D.Backorder.push_back(0);
    W.Backorder.push_back(0);
    R.Backorder.push_back(0);
    M.Received.push_back(0);//Initial Received at time zero
    D.Received.push_back(0);
    W.Received.push_back(0);
    R.Received.push_back(0);
    M.Sent.push_back(0);//Initial Sent at time zero
    D.Sent.push_back(0);
    W.Sent.push_back(0);
    R.Sent.push_back(0);
    R.Demand.push_back(0);
    M.Demand.push_back(0);
    D.Demand.push_back(0);
    W.Demand.push_back(0);
    R.Demand.push_back(10);//D(1) initialized at 10, consistend with end consumer order of 10 at time zero.
    i=0;
    while(i<=2001){
       R.Demand.push_back(9);
       i++;
       R.Demand.push_back(12);
       i++;
       R.Demand.push_back(8);
       i++;
       R.Demand.push_back(11);
       i++;       
    }
    i=0;
    if(L==0){M.Ord_Not_Rec.push(10);
        D.Ord_Not_Rec.push(10);
        W.Ord_Not_Rec.push(10);
        R.Ord_Not_Rec.push(10);
    }
    if(L!=0){
        i=0;
        while(i<L){M.Ord_Not_Rec.push(10);
            D.Ord_Not_Rec.push(10);
            W.Ord_Not_Rec.push(10);
            R.Ord_Not_Rec.push(10);
            i=i+1;
        }
    }
    i=0;

    while(i<=L && L!=0 ){
        M.Received.push_back(10);
        M.OHI[M.OHI.size()-1]+=M.Received[M.Received.size()-1];//OHI here is modification of last period's, not new elements
        W.Received.push_back(10);
        W.OHI[W.OHI.size()-1]+=W.Received[W.Received.size()-1];
        D.Received.push_back(10);
        D.OHI[D.OHI.size()-1]+=D.Received[D.Received.size()-1];       
        R.Received.push_back(10);
        R.OHI[R.OHI.size()-1]+=R.Received[R.Received.size()-1];       
        M.Demand.push_back(D.Order[D.Order.size()-1]);
        D.Demand.push_back(W.Order[W.Order.size()-1]);
        W.Demand.push_back(R.Order[R.Order.size()-1]);
        //Manufacturer will send goods after this.
        if(M.OHI[i]>=(M.Demand[i+1]+M.Backorder[i])){
            M.Sent.push_back(M.Demand[i+1]+M.Backorder[i]);
            M.OHI.push_back(M.OHI[M.OHI.size()-1]-M.Demand[i+1]-M.Backorder[i]);
            M.Backorder.push_back(0);
        }
        else{
            M.Sent.push_back(M.OHI[i]);
            M.OHI.push_back(0);
            M.Backorder.push_back(M.Backorder[i]+M.Demand[i+1]-M.OHI[i]);
        }
        //Distributor will send goods after this
        if(D.OHI[i]>=(D.Demand[i+1]+D.Backorder[i])){
            D.Sent.push_back(D.Demand[i+1]+D.Backorder[i]);
            D.OHI.push_back(D.OHI[D.OHI.size()-1]-D.Demand[i+1]-D.Backorder[i]);
            D.Backorder.push_back(0);
        }
        else{
            D.Sent.push_back(D.OHI[i]);
            D.OHI.push_back(0);
            D.Backorder.push_back(D.Backorder[i]+D.Demand[i+1]-D.OHI[i]);
        }
        //Wholesaler will send goods after this.
       if(W.OHI[i]>=(W.Demand[i+1]+W.Backorder[i])){
            W.Sent.push_back(W.Demand[i+1]+W.Backorder[i]);
            W.OHI.push_back(W.OHI[W.OHI.size()-1]-W.Demand[i+1]-W.Backorder[i]);
            W.Backorder.push_back(0);
        }
        else{
            W.Sent.push_back(W.OHI[i]);
            W.OHI.push_back(0);
            W.Backorder.push_back(W.Backorder[i]+W.Demand[i+1]-W.OHI[i]);
        }
        //Finally, Retailer
        if(R.OHI[i]>=(R.Demand[i+1]+R.Backorder[i])){
            R.Sent.push_back(R.Demand[i+1]+R.Backorder[i]);
            R.OHI.push_back(R.OHI[R.OHI.size()-1]-R.Demand[i+1]-R.Backorder[i]);
            R.Backorder.push_back(0);
        }
        else{
            R.Sent.push_back(R.OHI[i]);
            R.OHI.push_back(0);
            R.Backorder.push_back(R.Backorder[i]+R.Demand[i+1]-R.OHI[i]);
        }

        //Need to push orders into queue after here
//        pushpop(M.Ord_Not_Rec,M.Order[i]);
  //      pushpop(D.Ord_Not_Rec,D.Order[i]);
    //    pushpop(W.Ord_Not_Rec,W.Order[i]);
      //  pushpop(R.Ord_Not_Rec,R.Order[i]);
        //Update IP after this
//        M.IP.push_back(M.OHI[M.OHI.size()-1]+queue_sum(M.Ord_Not_Rec));
  //      D.IP.push_back(D.OHI[D.OHI.size()-1]+queue_sum(D.Ord_Not_Rec));        
    //    W.IP.push_back(W.OHI[W.OHI.size()-1]+queue_sum(W.Ord_Not_Rec));
      //  R.IP.push_back(R.OHI[R.OHI.size()-1]+queue_sum(R.Ord_Not_Rec));


         M.IP.push_back(M.IP[M.IP.size()-1]+M.Order[M.Order.size()-1]-M.Demand[i+1]);
         D.IP.push_back(D.IP[D.IP.size()-1]+D.Order[D.Order.size()-1]-D.Demand[i+1]); 
         W.IP.push_back(W.IP[W.IP.size()-1]+W.Order[W.Order.size()-1]-W.Demand[i+1]);
         R.IP.push_back(R.IP[R.IP.size()-1]+R.Order[R.Order.size()-1]-R.Demand[i+1]);


        
        //Generate Orders
        //ES1
        M.Order.push_back(ES2(M.Demand,M.IP,alpha,M.Forecast,i));
        D.Order.push_back(ES2(D.Demand,D.IP,alpha,D.Forecast,i));
        W.Order.push_back(ES2(W.Demand,W.IP,alpha,W.Forecast,i));
        R.Order.push_back(ES2(R.Demand,R.IP,alpha,R.Forecast,i));
         pushpop(M.Ord_Not_Rec,M.Order[M.Order.size()-1]);
        pushpop(D.Ord_Not_Rec,D.Order[D.Order.size()-1]);
        pushpop(W.Ord_Not_Rec,W.Order[W.Order.size()-1]);
        pushpop(R.Ord_Not_Rec,R.Order[D.Order.size()-1]);
        i++; 
    }
    
    while(i<=2001){//i is already at t=L, now main transactions start
        M.Received.push_back(M.Order[i-L+1]);
        M.OHI[i]+=M.Received[M.Received.size()-1];
        D.Received.push_back(M.Sent[i-L+1]);
        D.OHI[i]+=D.Received[D.Received.size()-1];
        W.Received.push_back(D.Sent[i-L+1]);
        W.OHI[i]+=W.Received[W.Received.size()-1];
        R.Received.push_back(W.Sent[i-L+1]);
        R.OHI[i]+=R.Received[R.Received.size()-1];
        M.Demand.push_back(D.Order[D.Order.size()-1]);
        D.Demand.push_back(W.Order[W.Order.size()-1]);
        W.Demand.push_back(R.Order[R.Order.size()-1]);
        
        if(M.OHI[M.OHI.size()-1]>=(M.Demand[M.Demand.size()-1]+M.Backorder[M.Backorder.size()-1])){
            M.Sent.push_back(M.Demand[M.Demand.size()-1]+M.Backorder[M.Backorder.size()-1]);
            M.OHI.push_back(M.OHI[i]-M.Demand[i+1]-M.Backorder[i]);
            M.Backorder.push_back(0);
        }
        else{
            M.Sent.push_back(M.OHI[i]);
            M.OHI.push_back(0);
            M.Backorder.push_back(M.Backorder[i]+M.Demand[i+1]-M.OHI[i]);
        }
        //Distributor will send goods after this
        if(D.OHI[i]>=(D.Demand[i+1]+D.Backorder[i])){
            D.Sent.push_back(D.Demand[i+1]+D.Backorder[i]);
            D.OHI.push_back(D.OHI[i]-D.Demand[i+1]-D.Backorder[i]);
            D.Backorder.push_back(0);
        }
        else{
      D.Sent.push_back(D.OHI[i]);
            D.OHI.push_back(0);
            D.Backorder.push_back(D.Backorder[i]+D.Demand[i+1]-D.OHI[i]);
        }
        //Wholesaler will send goods after this.
       if(W.OHI[i]>=(W.Demand[i+1]+W.Backorder[i])){
            W.Sent.push_back(W.Demand[i+1]+W.Backorder[i]);
            W.OHI.push_back(W.OHI[i]-W.Demand[i+1]-W.Backorder[i]);
            W.Backorder.push_back(0);
        }
        else{
            W.Sent.push_back(W.OHI[i]);
            W.OHI.push_back(0);
            W.Backorder.push_back(W.Backorder[i]+W.Demand[i+1]-W.OHI[i]);
        }
        //Finally, Retailer
        if(R.OHI[i]>=(R.Demand[i+1]+R.Backorder[i])){
            R.Sent.push_back(R.Demand[i+1]+R.Backorder[i]);
            R.OHI.push_back(R.OHI[i]-R.Demand[i+1]-R.Backorder[i]);
            R.Backorder.push_back(0);
        }
        else{
            R.Sent.push_back(R.OHI[i]);
            R.OHI.push_back(0);
            R.Backorder.push_back(R.Backorder[R.Backorder.size()-1]+R.Demand[i+1]-R.OHI[i]);
        }

        //Push and pop
        
//        pushpop(M.Ord_Not_Rec,M.Order[M.Order.size()-1]);
  //      pushpop(D.Ord_Not_Rec,D.Order[D.Order.size()-1]);
    //    pushpop(W.Ord_Not_Rec,W.Order[W.Order.size()-1]);
      //  pushpop(R.Ord_Not_Rec,R.Order[R.Order.size()-1]);
        
        //Update IP after this
//        M.IP.push_back(M.OHI[M.OHI.size()-1]+queue_sum(M.Ord_Not_Rec));// Old equation does not work, so commented
  //      D.IP.push_back(D.OHI[D.OHI.size()-1]+queue_sum(D.Ord_Not_Rec));        
    //    W.IP.push_back(W.OHI[W.OHI.size()-1]+queue_sum(W.Ord_Not_Rec));
      //  R.IP.push_back(R.OHI[R.OHI.size()-1]+queue_sum(R.Ord_Not_Rec));
        //Generate Orders
        //ES1

         M.IP.push_back(M.IP[M.IP.size()-1]+M.Order[M.Order.size()-1]-M.Demand[i+1]);
         D.IP.push_back(D.IP[D.IP.size()-1]+D.Order[D.Order.size()-1]-D.Demand[i+1]); 
         W.IP.push_back(W.IP[W.IP.size()-1]+W.Order[W.Order.size()-1]-W.Demand[i+1]);
         R.IP.push_back(R.IP[R.IP.size()-1]+R.Order[R.Order.size()-1]-R.Demand[i+1]);


        M.Order.push_back(ES22(M.Demand,M.IP,alpha,M.Forecast,i));
        D.Order.push_back(ES22(D.Demand,D.IP,alpha,D.Forecast,i));
        W.Order.push_back(ES22(W.Demand,W.IP,alpha,W.Forecast,i));
        R.Order.push_back(ES22(R.Demand,R.IP,alpha,R.Forecast,i));        
//        pushpop(M.Ord_Not_Rec,M.Order[M.Order.size()-1]);
  //      pushpop(D.Ord_Not_Rec,D.Order[D.Order.size()-1]);
    //    pushpop(W.Ord_Not_Rec,W.Order[W.Order.size()-1]);
      //  pushpop(R.Ord_Not_Rec,R.Order[R.Order.size()-1]);
        i++;
    }
   int l=7; 
        cout<<"Demand ="<<R.Demand[l]<<", Order ="<<R.Order[l]<<", Received = "<<R.Received[l]<<", IP = "<<R.IP[l]<<", Forecast ="<<R.Forecast[l]<<", Sent = "<<R.Sent[l]<<", OHI ="<<R.OHI[l]<<endl;
        fM<<alpha<<"\t";
        for(int r=0;r<=2000;r++){
            M.Totalinv.push_back(M.Backorder[r]+M.OHI[r]);
            D.Totalinv.push_back(D.Backorder[r]+D.OHI[r]);
            W.Totalinv.push_back(W.OHI[r]+W.Backorder[r]);
            R.Totalinv.push_back(R.OHI[r]+R.Backorder[r]); 
        }
        fM<<stddev(M.Order)<<endl;
        fD<<stddev(D.Order)<<endl;
        fW<<stddev(W.Order)<<endl;
        fR<<stddev(R.Order)<<endl;
        for(int k=0;k<=M.Order.size()-2;k++){
        //fM<<M.Order[k]<<endl;
        //fD<<D.Order[k]<<endl;
        //fW<<W.Order[k]<<endl;
        //fR<<R.Order[k]<<endl;
        }
        alpha+=0.01;
        cout<<"alpha="<<alpha<<endl;
        j=0;
        while(!R.Ord_Not_Rec.empty())R.Ord_Not_Rec.pop();
        while(!M.Ord_Not_Rec.empty())M.Ord_Not_Rec.pop();
        while(!D.Ord_Not_Rec.empty())D.Ord_Not_Rec.pop();
        while(!W.Ord_Not_Rec.empty())W.Ord_Not_Rec.pop();
        //cout<<"OHI="<<M.OHI[9]<<endl<<"std="<<stddev(M.OHI);
        //Destroy Older Values
        M.Demand.clear();
        M.OHI.clear();
        M.IP.clear();
        M.Backorder.clear();
        M.Order.clear();
        M.Forecast.clear();
        M.Received.clear();
        M.Sent.clear();
        M.Ord_Not_Rec=queue<double> ();
        D.Demand.clear();
        D.OHI.clear();
        D.IP.clear();
        D.Backorder.clear();
        D.Order.clear();
        D.Forecast.clear();
        D.Received.clear();
        D.Sent.clear();
        D.Ord_Not_Rec=queue<double> ();
        W.Demand.clear();
        W.OHI.clear();
        W.IP.clear();
        W.Backorder.clear();
        W.Order.clear();
        W.Forecast.clear();
        W.Received.clear();
        W.Sent.clear();
        W.Ord_Not_Rec=queue<double> ();
        R.Demand.clear();
        R.OHI.clear();
        R.IP.clear();
        R.Backorder.clear();
        R.Order.clear();
        R.Forecast.clear();
        R.Received.clear();
        R.Sent.clear();
        R.Ord_Not_Rec=queue<double> ();
        

    }
    /*/Testing block start
    *///Testing block end
    queue<double> Q;
    Q.push(20.0);
    Q.push(40.0);
    Q.push(30.0);
    pushpop(Q,1.0);
    cout<<Q.front()<<endl;
    Q.pop();
    cout<<Q.front()<<endl;
    Q.pop();
    cout<<Q.front()<<endl;
    Q.pop();

    return 0;
}
 
