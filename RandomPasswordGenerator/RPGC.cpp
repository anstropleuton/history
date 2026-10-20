#include<iostream>
#include<cstdlib>
#include<sstream>
#include<fstream>
using namespace std;
int v1(string v2){stringstream v4;
int v3=0,z;
bool v5=false;
for(z=0;z<v2.length();z++){if(v2[z]>='0'&&v2[z]<='9')v4<<v2[z];
if(v2[z]=='-')v5=true;}
v4>>v3;
if(v5)v3=v3*-1;
return  v3;}
void v6(bool v7,bool v8,int v9,int v10[3],char v11,char v12,stringstream &v3){int z;
if(v7){if(v8){for(z=0;z<v9;z++){if(z==v10[0]||z==v10[1]||z==v10[2])v3<<v11;
else v3<<v12;}}
else for(z=0;z<v9;z++)cout<<v12;
v3<<'\n';}}
string v15(int v2){stringstream v4;
string v3;
int v13[2147483647],v14=0,z;
while(v2){v13[v14]=v2%26;
v2=v2/26;
v14++;}
for(z=0;z<v14-1;z++){if(v13[z]<=0){v13[z]+=26;
v13[z+1]=v13[z+1]-1;}}
for(z=v14;z>=0;z--){if(v13[z]>0)v4<<char ('A'+v13[z]-1);}
v3=v4.str();
return  v3;}
bool v16(string v2,char v19,int &v17){bool v3=false;
v17++;
if(v2[(v17-1)]!=v19){if(v2[(v17-1)]){v3=true;}}
return  v3;}
string v18(string v2,char v19,char v20,char v21,int &v17){string v3;
stringstream v4;
if(v2[v17]!=v19){if(v2[v17]==v20){while(v2[(v17+1)]!=v21){v17++;
v4<<v2[v17];}
v3=v4.str();
v4.str("");
v17++;}}
v17++;
return  v3;}
char v22(string v2,char v19,char v20,char v21,int &v17){char v3;
stringstream v4;
if(v2[v17]!=v19){v17++;
v3=v2[v17];
v17++;}
v17++;
return  v3;}
int v23(string v2,char v19,char v20,char v21,int &v17){int v3;
stringstream v4;
if(v2[v17]!=v19){if(v2[v17]==v20){while(v2[(v17+1)]!=v21){v17++;
v4<<v2[v17];}
v4>>v3;
v17++;}}
v17++;
return  v3;}
int main(){string v24,v25="help",v26="n",v27,v28,v29="C:/Generated_Passwords.txt",v30="/storage/emulated/0/Generated_Passwords.txt",v31,v32;
char v33='<',v34='>',v35='!',v36='-',v37='[',v38=']',v39='|',v40='+';
int v41=1,v42=1,v43=0,v44=time(0);,v45[3],v46,v47=3,z;
bool v48=false,v49=false,v50=true,v51=false,v52=true,v53=false,v54=true,v55=true,v56=false,v57=false,v58=false;
int v59=v42;
stringstream v60,v90,v3,v61,v62;
string v65[]={"Howlongyouwantyourpasswordtobe?\n",
"InvalidNumber,Therehastobemorethan0char acters.Tryagain.\n",
"Howmanypasswordsyouwanttogenerate?\n",
"InvalidNumber,Therecannotbelessthan1passwords.Tryagain.\n",
"v7Drawisafeaturethatdrawsv7smadeofyourchoiceatthestartofgeneratingpasswordandattheendofgeneratedpasswords.Thisisjustadecorationfeature!\n",
"Thisisthefeaturethataddsav14v13ofpasswordsgenerated.e.g.\n(1):<password>\n(2):<password>\n...Etc.Thisfeatureisalsocustomization!\n",
"Thisfeaturedrawsv7sverticallytomakethepasswordsorganizedlikeaexcelspreadsheet.Butitrequires2char acterstouse.Onefortheverticalv7andanotherforthev11usedtoconnecthorizontalv7.Thehorizontalv7isv7v12ifyouenteredbefore.Thisfeatureisalsofordecoration!\n",
"Thev8char acterusedtomakev8e.g.'|'.\n",
"Thisfeatureuseschar actersfromv20iousfeatures,\"v7Draw\"andthechar acterisusedfrom\"v7Drawv12\"anditseemslikeyoudidnotusedthatfeature,enterthechar acterfortheprogramtousetomakev8e.g.'-'.\n",
"Thev11char acterusedtoconnectverticalv7andhorizontalv7e.g.'+'.\n",
"Adjustsallthesettingsautomaticallysoit'sstableforthegenerator(Warning,Ifyoudon'tusethis,chancesaretheprogramwillbeunstableormightCrash).\n",
"v59isthefeaturethatdrawsv7usingchar acters.Thisv13determineshowmanychar acterstouse.\n",
"v45isthesettingthatisusedtocheckifthematchingconditionsaremetforthev40tobeused.\n",
"Uselowercasedchar actersaspasswordrandomletters?\n",
"Useuppercasedchar actersaspasswordrandomletters?\n",
"Usenumbersaspasswordrandomnumbers?\n",
"Seeddetermineshowyourpasswordisgonnalook.Thelastletter\"Seed:<number>\"istheseedforcurrentpassword.Youcanshareseedbutbetternotbecauseotherscangenerateexactsamepasswordsjustlikeyours.Ifyoufoundsomethingcoolandyouaren'tusingthisprogramforpasswordthenfeelfreetoshareseedaswellaspasswordv9andv14asthat'snecessarytogenerateexactsamepasswords.The't'meanstousethesystemtimeastheseedforrandompasswordstogenerate.\n",
"Changesettingsthatisusedtogeneratepassword(Warning!Insertinga'Space'char actermightcrashtheprogram)Youcanalsov2allthesettingsatonceifyoulikeandtheprogramwilldetectthemandusethemaccordingly(thefirstchar acteryoutypeasv10isgoingtobethestartofastring v13andthev21isendofthestring thenyoucanusethemwhileenteringstring v10.e.g.youentered\"<>\"thenwheneveryouuse'<'theprogramwilldetectthatandusesitatstartofthestring andthenitwillsearchfor'>'andthenitwillconcludethev13andsetsupthev21andthethirdchar acterwillbev19char acter.Thenwhat'sleftisallv10,typejust'o'togetorder).\n",
"Restartallofthecode(Doesn'tresetsthev13sandsettingsunlessselected'r'whichbringseverythingtodefault)The'd'sendsalltheinformationthecodehasusedandallthesettingsv13sandstuffsfordebugpurposes,The's'sendsthev2debugusedbyv25.The'o'tov3thepasswordsint oafilewhichwillbecreatedin'/storage/emulated/0/'or'C:/'.Youcanalsoreadcontentsfromthefilemytyping'i'ifyoudon'tknowwhatexistsinit!\n",
"Someoptionalfeaturestoenabletomakeyourpasswordslookalittlebitfancier!\n",
"Ordersofthev10susedinsettings(fillint herestwithv19char acter)\n\nv33;\nv34;\nv35;\nv48;\nv36;\nv49;\nv50;\nv37;\nv38;\nv51;\nv39;\nv40;\nv53;\nv54;\nv55;\nv52;\nv59;\nv45[0];\nv45[1];\nv45[2];\n\n",
"Thischar acterisusedbeforethepasswordNumbersorLettersareshown.\n",
"Thischar acterisusedafterthepasswordNumbersorLettersareshown.\n"};
cout<<"Notethatyoucanuse\"help\"anywhereint hev2int heprogramtogetabriefhelpaboutit!Andyoucanuse\"back\"ifyouwanttogobacktov20iousconversation!"<<endl;
v63:
v3.str("");
v64:cout<<"Passwordv9(1+):";
cin>>v26;
if(v26=="help"){cout<<v65[0];
goto v64;}
else if(v26=="back"){cout<<"Thereisnostepback."<<endl;
goto v64;}
else v41=v1(v26);
if(v41<=0){cout<<v65[1];
goto v64;}
v66:cout<<"Passwordsv14(1+):";
cin>>v26;
if(v26=="help"){cout<<v65[2];
goto v66;}
else if(v26=="back")goto v64;
else v42=v1(v26);
if(v42<=0){cout<<v65[3];
goto v66;}v59=v42;
v67:cout<<"ChangeSettings(y/n/o/<v10>):";
cin>>v25;
if(v25=="y"){v68:cout<<"Enablefeatures(y/n):";
cin>>v26;
if(v26=="y"){v69:cout<<"v7Draw(y/n):";
cin>>v26;
if(v26=="y"){v70:cout<<"v7v12e.g.'-':";
cin>>v26;
if(v26=="help"){cout<<v65[4];
goto v70;}
else if(v26=="back")goto v69;
else v36=v26[0];
cout<<"v7v12mightlooklikethis:"<<endl;
for(z=0;z<20;z++)cout<<v36;
cout<<endl<<"Confirm(y/n):";
cin>>v26;
if(v26!="y"){cout<<"TryAgain:"<<endl;
goto v70;}
else if(v26=="help"){cout<<v65[4];
goto v69;}
else v48=true;}
else if(v26=="back")goto v68;
else if(v26=="help"){cout<<v65[4];
goto v69;}
else v48=false;
v71:cout<<"Passwordv17(y/n):";
cin>>v26;
if(v26=="y"){v72:cout<<"UseNumbers/Letters(n/l):";
cin>>v26;
if(v26=="l"){v50=false;
v27="Letter";
cout<<"ConfiguredtouseLetters!"<<endl;}
else {v50=true;
v27="Number";
cout<<"ConfiguredtouseNumbers!"<<endl;}
v73:cout<<"Before"<<v27<<"e.g.'[':";
cin>>v26;
if(v26=="help"){cout<<v65[21];
goto v73;}
else if(v26=="back")goto v72;
else if(v26=="none")v37='\0';
else v37=v26[0];
v74:cout<<"After"<<v27<<"e.g.']':";
cin>>v26;
if(v26=="help"){cout<<v65[22];
goto v74;}
else if(v26=="back")goto v73;
else if(v26=="none")v38='\0';
else v38=v26[0];
if(v50)v27="20";
else v27="AB";
cout<<"Passwordv14mightlooklikethis:"<<endl<<v37<<v27<<v38<<"a1b2c3x7y8z9"<<endl<<"Confirm(y/n):";
cin>>v26;
if(v26=="help"){cout<<v65[5];
goto v71;}
else if(v26=="back")goto v74;
else if(v26!="y"){cout<<"TryAgain."<<endl;
goto v72;}
else v49=true;}
else if(v26=="back")goto v69;
else if(v26=="help"){cout<<v65[5];
goto v71;}
else v49=false;
v75:cout<<"v8v7(y/n):";
cin>>v26;
if(v26=="y"){v76:cout<<"v8char actere.g.'|':";
cin>>v26;
if(v26=="help"){cout<<v65[7];
goto v76;}
else if(v26=="back")goto v75;
else v39=v26[0];
if(!v48){v77:cout<<"Horizontalv7e.g.'-':";
cin>>v26;
if(v26=="help"){cout<<v65[8];
goto v77;}
else if(v26=="back")goto v76;
else v36=v26[0];}
else cout<<"Usingv7Drawchar acterasHorizontalv7."<<endl;
v78:cout<<"v11char actere.g.'+':";
cin>>v26;
if(v26=="help"){cout<<v65[9];
goto v78;}
else if(v26=="back")goto v76;
else v40=v26[0];
cout<<v40;
for(z=0;z<12;z++)cout<<v36;
cout<<v40<<endl<<v39<<"a1b2c3x7y8z9"<<v39<<endl<<v40;
for(z=0;z<12;z++)cout<<v36;
cout<<v40<<endl<<"Confirm(y/n):";
cin>>v26;
if(v26=="help"){cout<<v65[6];
goto v75;}
else if(v26=="back")goto v78;
else if(v26!="y"){cout<<"TryAgain:";
goto v76;}
else v51=true;}
else if(v26=="back")goto v71;
else if(v26=="help"){cout<<v65[6];
goto v75;}
else v51=false;}
else if(v26=="back")goto v67;
else if(v26=="help"){cout<<v65[19];
goto v68;}
v79:cout<<"Upperchar acters(y/n):";
cin>>v26;
if(v26=="y")v53=true;
else if(v26=="back")goto v68;
else if(v26=="help"){cout<<v65[13];
goto v79;}
else v54=false;
v80:cout<<"Lowerchar acters(y/n):";
cin>>v26;
if(v26=="y")v54=true;
else if(v26=="help"){cout<<v65[14];
goto v80;}
else v54=false;
if(v53||v54){v81:cout<<"Numbers(y/n):";
cin>>v26;
if(v26=="y")v55=true;
else if(v26=="help"){cout<<v65[15];
goto v81;}
else v55=false;}
v82:cout<<"GeneratorSeed(<seed>/t):";
cin>>v26;
if(v26=="help"){cout<<v65[16];
goto v82;}
else if(v26=="t")v44=time(0);
else v44=v1(v26);
v83:cout<<"AutoAdjust(veryrecommended)(y/n):";
cin>>v26;
if(v26=="y"){v52=true;}
else if(v26=="help"){cout<<v65[10];
goto v83;}
else {if(v48){cout<<"v59=";
v84:cin>>v26;
if(v26=="help")cout<<v65[11];
else v59=v1(v26);}
if(v51){cout<<"v45[0]=";
v85:cin>>v26;
if(v26=="help"){cout<<v65[12];
goto v85;}
else v45[0]=v1(v26);
cout<<"v45[1]=";
v86:cin>>v26;
if(v26=="help"){cout<<v65[12];
goto v86;}
else v45[1]=v1(v26);
cout<<"v45[2]=";
v87:cin>>v26;
if(v26=="help"){cout<<v65[12];
goto v87;}
else v45[2]=v1(v26);}}}
else if(v25=="back")goto v66;
else if(v25=="help"){cout<<v65[17];
goto v67;}
else if(v25=="o"){cout<<v65[20];
goto v67;}
else if(v25!="n"){v56=true;
goto v89;
v88:cout<<"ConfigureSuccess!"<<endl;}
if(v52){if(v49){v59=v41+1;
if(v38!='\0')v59++;
if(v37!='\0')v59++;}
else v59=v41;
if(v51){v59+=2;
if(v49){v59+=1;
v45[0]=0;
v45[1]=2;
if(v38!='\0')v45[1]++;
if(v37!='\0')v45[1]++;
v45[2]=(v45[1]+v41+1);}
else {v45[0]=0;
v45[1]=-1;
v45[2]=(v41+1);}}
else {v45[0]=-1;
v45[1]=-1;
v45[2]=-1;}}
for(z=0;z<62;z++){if(z<10&&v55)v60<<char (z+48);
if(z<36&&z>9&&v53)v60<<char (z+55);
if(z>35&&v54)v60<<char (z+61);}
v24=v60.str();
srand(v44);
cout<<endl;
if(v51&&!v48)v6(true,v51,v59,v45,v40,v36,v3);
else v6(v48,v51,v59,v45,v40,v36,v3);
for(int a=0;a<v42;a++){if(v49){if(v51)v3<<v39;
v3<<v37;
if(v50){v3<<(a+1);
v90.str("");
v90<<(a+2);
v46=v90.str().length()-1;}
else {v28=v15((a+1));
v3<<v28;
v28=v15((a+2));
v46=v28.length()-1;}
v3<<v38;
if(v51)v3<<v39;}
else if(v51)v3<<v39;
for(int b=0;b<v41;b++){v43=rand()%v24.length();
v3<<v24[v43];}
if(v51)v3<<v39;
if(v49){v45[1]+=v46;
v45[2]+=v46;
v59+=v46;}
v3<<'\n';
if(v48||v51){if(a+1==v42)v6(true,v51,v59,v45,v40,v36,v3);
else v6(v48,v51,v59,v45,v40,v36,v3);}
if(v49){v45[1]-=v46;
v45[2]-=v46;
v59-=v46;}}
cout<<v3.str()<<endl<<"Seed:"<<v44<<endl;
v91:cout<<"Restart(y/n/d/r/s/i/o):";
cin>>v26;
if(v26=="y")goto v63;
else if(v26=="help"){cout<<v65[18];
goto v91;}
if(v26=="back"){cout<<"Cannotchangesettingsafterpasswordshasbeedgenerated."<<endl;
goto v91;}
else if(v26=="d"){cout<<"registerint z:"<<z<<endl;
cout<<"int v41:"<<v41<<endl;
cout<<"int v42:"<<v42<<endl;
cout<<"int v43:"<<v43<<endl;
cout<<"int v44:"<<v44<<endl;
cout<<"int v45[0]:"<<v45[0]<<endl;
cout<<"int v45[1]:"<<v45[1]<<endl;
cout<<"int v45[2]:"<<v45[2]<<endl;
cout<<"int v46:"<<v46<<endl;
cout<<"bool v48:"<<v48<<endl;
cout<<"bool v49:"<<v49<<endl;
cout<<"bool v50:"<<v50<<endl;
cout<<"bool v51:"<<v51<<endl;
cout<<"bool v52:"<<v52<<endl;
cout<<"bool v53:"<<v53<<endl;
cout<<"bool v54:"<<v54<<endl;
cout<<"bool v55:"<<v55<<endl;
cout<<"bool v56:"<<v56<<endl;
cout<<"string v24:"<<v24<<endl;
cout<<"string v25:"<<v25<<endl;
cout<<"string v26:"<<v26<<endl;
cout<<"string v27:"<<v27<<endl;
cout<<"string v36:"<<v36<<endl;
cout<<"string v37:"<<v37<<endl;
cout<<"string v38:"<<v38<<endl;
cout<<"string v39:"<<v39<<endl;
cout<<"string v40:"<<v40<<endl;
cout<<"string v28:"<<v28<<endl;
cout<<"stringstream v60:"<<v60.str()<<endl;
cout<<"stringstream v90:"<<v90.str()<<endl;
goto v91;}
else if(v26=="r"){z=0;
v41=0;
v42=0;
v43=0;
v44=time(0);
v45[0]=0;
v45[1]=0;
v45[2]=0;
v46=0;
v48=false;
v49=false;
v50=true;
v51=false;
v52=true;
v53=false;
v54=true;
v55=true;
v56=false;
v60.str("");
v90.str("");
v24.clear();
v25="help";
v26="n";
v27="n";
v36='-';
v37='[';
v38=']';
v39='|';
v40='+';
v33='<';
v34='>';
v35='!';
v57=false;
v58=false;
v28.clear();
system("clear");
goto v63;}
else if(v26=="s"){cout<<"char v33:"<<v33<<endl;
cout<<"char v34:"<<v34<<endl;
cout<<"char v35:"<<v35<<endl;
cout<<"bool v48:"<<v48<<endl;
cout<<"string v36:"<<v36<<endl;
cout<<"bool v49:"<<v49<<endl;
cout<<"bool v50:"<<v50<<endl;
cout<<"string v37:"<<v37<<endl;
cout<<"string v38:"<<v38<<endl;
cout<<"bool ftrv8Lin:"<<v51<<endl;
cout<<"string v39:"<<v39<<endl;
cout<<"string v51Do:"<<v40<<endl;
cout<<"bool v53:"<<v53<<endl;
cout<<"bool setupLowe:"<<v54<<endl;
cout<<"bool v55:"<<v55<<endl;
cout<<"bool v52:"<<v52<<endl;
cout<<"int v59:"<<v59<<endl;
cout<<"int v45[0]:"<<v45[0]<<endl;
cout<<"int v45[1]:"<<v45[1]<<endl;
cout<<"int v45[2]:"<<v45[2]<<endl;
cout<<"v10Used:"<<v25<<endl;
goto v91;}
else if(v26=="i"){fstream v92("/storage/emulated/0/Generated_Passwords.txt");
if(v92.fail()){v92.open("C:/Generated_Passwords.txt");
if(v92.fail()){cout<<"Failedtoviewafile."<<endl;
v92.close();
goto v91;}}
cout<<"ReadingPasswords..."<<endl<<endl;
for(z=0;getline(v92,v27);z++)cout<<v27<<endl;
v92.close();
cout<<endl<<z<<"v7sFound"<<endl;
goto v91;}
else if(v26=="o"){ofstream v93,v94;
ifstream v95;
v95.open(v30.c_str());
if(v95.fail()){v95.open(v29.c_str());
if(v95.fail()){v95.close();
v93.open(v30.c_str());
if(v93.fail()){v93.open(v29.c_str());if(v93.fail()){cout<<"Failedtocreateafile"<<endl;v93.close();goto v91;}}
v93<<v3.str();
v93.close();}}
else {v62<<time(0);
v62>>v27;
v62.str("");
v31=("C:/"+v27+"_v61_Generated_Passwords.txt");
v32=("/storage/emulated/0/"+v27+"_v61_Generated_Passwords.txt");
cout<<"\"Generated_Passwords.txt\"found,Makingav61"<<endl;
v94.open(v32.c_str());
if(v94.fail()){v93.open(v31.c_str());if(v94.fail()){cout<<"Failedtov61afile"<<endl;v95.close();v94.close();goto v91;}}
while(getline(v95,v27))v94<<v27<<endl;
v95.close();
v94.close();
cout<<"v61edfile."<<endl;
v93.open(v30.c_str());
if(v93.fail()){v93.open(v29.c_str());if(v93.fail()){cout<<"Failedtostorepasswordsinafile"<<endl;goto v91;}}
v27.clear();
v93<<v3.str();
v93.close();}
cout<<"Successfullyv3tedPasswords!"<<endl<<"Locationofthefileiseitherat\""<<v30<<"\"ifyouareusingv30or\""<<v29<<"\"ifyouareusingv29."<<endl;
goto v91;}
else exit(1);
v89:if(v56){v33=v25[0];
v34=v25[1];
v35=v25[2];
v48=v16(v25,v35,v47);
v36=v22(v25,v35,v33,v34,v47);
v49=v16(v25,v35,v47);
v50=v16(v25,v35,v47);
v37=v22(v25,v35,v33,v34,v47);
v38=v22(v25,v35,v33,v34,v47);
v51=v16(v25,v35,v47);
v39=v22(v25,v35,v33,v34,v47);
v40=v22(v25,v35,v33,v34,v47);
v53=v16(v25,v35,v47);
v54=v16(v25,v35,v47);
v55=v16(v25,v35,v47);
v52=v16(v25,v35,v47);
v59=v23(v25,v35,v33,v34,v47);
v45[0]=v23(v25,v35,v33,v34,v47);
v45[1]=v23(v25,v35,v33,v34,v47);
v45[2]=v23(v25,v35,v33,v34,v47);
v56=false;
goto v88;}}