#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;
void checkNum(int&);
void checkNum(double& num);
void checkString(string &a);
void enter(int n, string** criteria, int pos);
void enterBasicData(int&, int&, int&, string**, string*);
void enterAlternatives(int n, string* alternative);
void criteriaEvaluation(double*, int, string**);
void subcriteriaEvaluation(double**, int, int, string**);
void alternativeEvaluation(double**, int, int, string*, string**, int);

int main() {
  string problem;
  cout << "Enter the name of problem ";
  getline(cin, problem);
  checkString(problem);
//  while(!(problem[0] >= 'A' && problem[0] <= 'Z' || problem[0] >= 'a' && problem[0] <= 'z')) {
//    cout<<"\nwrong name of problem ";
//    getline(cin, problem);
//  }

  //???? ???????? ??????
  int criteriaAmount, subcriteriaAmount, alternativeAmount;
  string** criteria;
  cout << "Enter the amount of criterias ";
  cin>>criteriaAmount;
  checkNum(criteriaAmount);
  criteria = new string*[criteriaAmount+1];
  string* alternative = new string[8];
  enterBasicData(criteriaAmount, subcriteriaAmount, alternativeAmount, criteria, alternative);
  //???????? ??????? ?????? ?????????
  double* criteriaEstimate;
  criteriaEstimate = new double[criteriaAmount];
  criteriaEvaluation(criteriaEstimate, criteriaAmount, criteria);

  //???????? ??????? ?????? ????????????
  double** subcriteriaEstimate = new double*[criteriaAmount];
  for(int i = 0; i < criteriaAmount; i++) {
    subcriteriaEstimate[i] = new double[subcriteriaAmount];
  }
  subcriteriaEvaluation(subcriteriaEstimate, criteriaAmount, subcriteriaAmount, criteria);

  int allSubcriterias = subcriteriaAmount * criteriaAmount;
  //???????? ??????? ?????? ???????????
  double** alternativeEstimate = new double*[allSubcriterias];
  for(int i = 0; i < allSubcriterias; i++) {
    alternativeEstimate[i] = new double[alternativeAmount];
  }
  alternativeEvaluation(alternativeEstimate, allSubcriterias, alternativeAmount, alternative, criteria, subcriteriaAmount);

  //???????? ??????? ????????????? ??????????? (???????? * ???????????)
  double* priorityFactor;
  priorityFactor = new double[allSubcriterias];

  for(int i = 0, k = 0; i < criteriaAmount; i++) {
    for(int j = 0; j < subcriteriaAmount; j++) {
      priorityFactor[k] = criteriaEstimate[i] * subcriteriaEstimate[i][j];
      k++;
    }
  }
  cout<<"\npriorityFactor\n";
  for(int i = 0; i < allSubcriterias; i++) {
    cout<<i<<" = "<<priorityFactor[i]<<"  ";
  }
  // delete[] criteriaEstimate;

  //???????? ??????? ???????? ??????????? ?? ????????????
  double** alternativeVectors = new double*[allSubcriterias];
  for(int i = 0; i < allSubcriterias; i++) {
    alternativeVectors[i] = new double[alternativeAmount];
  }

  for(int i = 0; i < allSubcriterias; i++){
    for(int j = 0; j < alternativeAmount; j++) {
      alternativeVectors[i][j] = alternativeEstimate[i][j] * priorityFactor[i];
    }
  }
  cout<<"\nalternativeVectors\n";
  for(int i = 0; i < allSubcriterias; i++){
    for(int j = 0; j < alternativeAmount; j++) {
      cout<<setprecision(3)<<setw(10)<<"[" << i + 1 << "][" << j + 1 << "]=" << alternativeVectors[i][j]<<setw(10);
    }
    cout<<"\n";
  }
  // for (int i = 0; i < allSubcriterias; i++)
  // {
  //   delete[] alternativeVectors[i];
  // }
  // delete[] alternativeVectors;
  // delete[] priorityFactor;

  double* globalPriorityFactor;
  globalPriorityFactor = new double[alternativeAmount];
  double max = 0; int num;
  for(int i = 0; i < alternativeAmount; i++) {
    globalPriorityFactor[i] = 0;
    for(int j = 0; j < allSubcriterias; j++) {
      globalPriorityFactor[i] += alternativeVectors[j][i];
      } 
    cout<<"\nThe estimate of Alternative: "<<alternative[i]<<" = "<<globalPriorityFactor[i];

    if(globalPriorityFactor[i] >= max) {
      max = globalPriorityFactor[i];
      num = i;
    }
  } 

  cout<<"\nThe best alternative is "<<alternative[num]<<" = "<<max<<"\n";
  for(int i = 0; i < allSubcriterias; i++) {
    delete[]alternativeVectors[i];
  }
  delete[]alternativeVectors;
  delete[]globalPriorityFactor;
  

  system("pause");
  return 0;
}

//==============================================================
//??????? ????? ????????? ????????
void enterBasicData(int &criteriaAmount, int &subcriteriaAmount, int &alternativeAmount, string** criteria, string* alternative) {
  
  enter(criteriaAmount, criteria, 0);
  
  cout << "\nEnter the amount of sub-criterias: ";
  cin >> subcriteriaAmount;
  checkNum(subcriteriaAmount);
    
  for(int i = 1; i <= criteriaAmount; i++) {
    cout<<"\nSub-criterias of  "<<criteria[0][i-1]<<"\n";
    enter(subcriteriaAmount, criteria, i);
  }

  cout << "\nEnter the amount of alternatives: ";
  cin >> alternativeAmount;
  checkNum(alternativeAmount);
  enterAlternatives(alternativeAmount, alternative);
}

void checkNum(int& num){
    while(true) {
    if(cin.fail()) {
      cin.clear();
      fflush(stdin); 
      cout<<"wrong value of entered data\n";
      cin>>num;
    }
    else break;
  }	
}
void checkNum(double& num){
	while(1) {
        if(cin.fail()) {
          cin.clear();
          cout<<"\twrong value of the criteria estimate(enter a number)\n";
          fflush(stdin);
          cin>>num;
        }
        if(!cin.fail()) {
          if(num >= 0 && num <= 10) break;
          else {
            cout<<"\tenter estimate between 0 and 10\n";
            cin.clear();
            fflush(stdin);
            cin>>num;
          }
        }
    }
}
void checkString(string &a){
	while(true) {
       for(int j = 0; j<a.length(); j++){
          if(!(a[j] >= 'A' && a[j] <= 'Z' || a[j] >= 'a' && a[j] <= 'z')) {
                cout<<"Wrong enetred data\n";
                cin.get();
                getline(cin, a);
                break;
           }
        }
        break;
     }
}
void enterAlternatives(int n, string* alternative)
{
  string a;
  for (int i = 1; i <= n; i++)
  {
    cout << "The name of " << i <<" ";
    cin.get();
    getline(cin, a); 
    checkString(a);
    alternative[i-1]=a;
   }
 }

void enter(int n, string** criteria, int pos)
{
  string a;
  criteria[pos] = new string[n];
  for (int i = 1; i <= n; i++)
  {
    cout << "The name of " << i <<" ";
    cin.get();
    getline(cin, a); 
    checkString(a);
    criteria[pos][i-1]=a;
   }
 }


void criteriaEvaluation(double* criteriaEstimate, int criteriaAmount, string** criteria) {
  cout << "\n\nPlase, estimate your criterias, sub-criterias and alternatives from 1 to 10.\n"<< "for example,\n 1 - the importance is very low,\n 5 - middle importance,\n 10 - the importance is very high";
  
  double sum = 0;
  cout.precision(3);

  for(int i = 0; i < criteriaAmount; i++) {
    cout<<"\n\tCriteria: "<<criteria[0][i]<<" = ";
    cin>>criteriaEstimate[i];
	checkNum(criteriaEstimate[i]);
    sum += criteriaEstimate[i];
  }

  for(int i = 0; i < criteriaAmount; i++) {
    criteriaEstimate[i] = criteriaEstimate[i] / sum;
  }
  
  cout << "\nThe evaluations of criterias";
  for(int i = 0; i < criteriaAmount; ++i) {
    cout<< "\n\tCriteria: " << criteria[0][i] << "=" <<criteriaEstimate[i]<<"  ";
  }
  cout<<endl;
}

//????, ?????? ? ????? ?????? ????????????
void subcriteriaEvaluation(double** subcriteriaEstimate, int criteriaAmount, int subcriteriaAmount, string** criteria) {
  double *sum;
  sum = new double[criteriaAmount];
  for(int i = 0; i < criteriaAmount; i++) {
    sum[i] = 0;
  }
  
  cout<<"\n\nEnter value of subcriteria for each criteria (from 1 to 10):";
  for(int i = 0; i < criteriaAmount; i++) {
    cout<<"\nCriteria: "<<criteria[0][i];
    for(int j = 0; j < subcriteriaAmount; j++) {
      cout<<"\n\tSubcriteria: "<<criteria[i+1][j]<<" = ";
      cin>>subcriteriaEstimate[i][j];
      checkNum(subcriteriaEstimate[i][j]);
    }
  }

  for(int i = 0; i < criteriaAmount; i++) {
    for(int j = 0; j < subcriteriaAmount; j++) {
      sum[i] += subcriteriaEstimate[i][j];
    }
  }

  for(int i = 0; i < criteriaAmount; i++) {
    for(int j = 0; j < subcriteriaAmount; j++) {
      subcriteriaEstimate[i][j] = subcriteriaEstimate[i][j] / sum[i];
    }
  }

  delete[]sum;
  cout<<endl;
  

  for(int i = 0; i < criteriaAmount; i++) {
    for(int j = 0; j < subcriteriaAmount; j++) {
      cout<<setprecision(3)<<setw(5)<<"[" << i + 1 << "][" << j + 1 << "]=" << subcriteriaEstimate[i][j]<<setw(5);
    }
    cout<<"\n";
  }
}

//????, ?????? ? ????? ?????? ?????????
void alternativeEvaluation(double** alternativeEstimate, int allSubcriterias, int alternativeAmount, string* alternative, string** criteria, int subCritAmmount) {
  cout<<"\n\n\nEnter the value of alternatives for each sub-criteria: \n";

  double* sum;
  sum = new double[allSubcriterias];

  for(int i = 0; i < allSubcriterias; i++) {
    sum[i] = 0;
  }
  int posX=1, posY=0;
  for(int i = 0; i < allSubcriterias; i++) {
    cout<<"\nsubcriteria: "<<criteria[posX][posY];
    posY++;
    if(subCritAmmount==posY){posY=0;posX++;}
    for(int j = 0; j < alternativeAmount; j++) {
      cout<<"\n\talternative: "<<alternative[j]<<" = ";
      cin>>alternativeEstimate[i][j];
      checkNum(alternativeEstimate[i][j]);
      sum[i] += alternativeEstimate[i][j];
    }
  }

  for(int i = 0; i < allSubcriterias; i++) {
    for(int j = 0; j < alternativeAmount; j++) {
      alternativeEstimate[i][j] = alternativeEstimate[i][j] / sum[i];
    }
  }

  delete[]sum;

  for(int i = 0; i < allSubcriterias; i++) {
    for(int j = 0; j < alternativeAmount; j++) {
      cout<<setprecision(3)<<setw(5)<<"[" << i + 1 << "][" << j + 1 << "]=" << alternativeEstimate[i][j]<<setw(5);
    }
    cout<<"\n";
  }
}
