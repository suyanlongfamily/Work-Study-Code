//---------------------------------------------------------------------------


#pragma hdrstop

#include "AUTD_pubFunction.h"
#include "AUTD_pubTranCode.h"
#include "AUTD_httpDeal.h"
#include "AUTD_QueryCurrent.h"
#include <math.h>


#pragma comment(lib,"json.lib")
#pragma link "ExtStringGrid"
#define GRID_WIDTH_ADJUST 0
//---------------------------------------------------------------------------

bool g_bGetPriceSucc = false;

const TColor GroupColor = (TColor)0x00F8F2F3;
const TColor GridFirstColor = TColor(0x00F4ECE5);
const TColor GridSecondColor = TColor(0x00F9F8F7);
const TColor GridSelectColor = TColor(RGB(4,27,183));

TMainMenu *g_MainMenu;
int g_iTodayOfTrading;

/**���ɽ�����������*/
Json::Value g_valInitQueryData;
//Json::Value g_valCustFundsInit;
bool g_bQueryCustFundsSucc = false;
//Json::Value g_valStorageInit;
bool g_bQueryStorageSucc = false;
//Json::Value g_valPositionInit;
bool g_bQueryPositionSucc = false;
bool g_bQueryTradeFeeRateSucc = false;
bool g_bQuerySysParamSucc = false;

int SysParamListPageNo = 1;
bool SysParamListNextPage = false;

int YesterdayRiskListPageNo = 1;
bool YesterdayRiskListNextPage = false;
bool g_bQueryYesterdayRiskSucc = false;
Json::Value valQueryYesterdayRisk;

//����ʧ��(��������)list
Json::Value g_valInfoUpdateList;


//��������
const AnsiString TradeTypeContext = "�ֻ�����,0;���ڽ���,2;�����걨,4;������,6";

//�������� (�ļ��µ�ר���Ӽ�,ֻ��������combobox)
const AnsiString TradeTypeReOrderContext = "�ֻ�����,0;���ڽ���,2";

//�г�����
const AnsiString MarkettypeContext = "�ֻ�,s;����,d;�ڻ�,f;Զ��,w";

//�ɽ���״̬
const AnsiString DeliveryStateContext = "���ɽ����������걨,0;�ɽ����������걨,1";

//��������
const AnsiString BuyOrSellContext = "����,b;����,s";

//��������&���շ��� (��ѯ�����⴦��)
const AnsiString BSJSContext = "����,b;����,s;�ջ�,i;����,o";

//���շ���
const AnsiString JiaoOrShouContext = "�ջ�,b;����,s";

//ί��״̬
const AnsiString OrderStateContext = "δ�ɽ�,o;���ֳɽ�,p;��ȫ�ɽ�,c;�û�ɾ��,d;ϵͳɾ��,s;���걨,i;�ѱ���,m;����ʧ��,n;���ڷ���,h";

//��ƽ�ֱ�־
const AnsiString OffsetFlagContext = "����,0;ƽ��,1;ǿ��ƽ��,2";
const AnsiString OffsetFlagContextR = "����,0;ƽ��,1";

//֧������
const AnsiString PayDirectionContext = "δ��,0;��֧����,1;��֧����,2;δ��,3";

//�걨״̬(�������)
const AnsiString ApplyStateContext = "�ɹ�,1;�����������,2;����������,3;������ѳ���,4;ʧ��,5;δ��,6";

//�����״̬
const AnsiString PickupApplyStateContext = "�������,1;��Ա���ճ���,3;��Ա���ճ���,4;ϵͳ����,5;����������,6;����������,7;\
    ����ƻ��Ѱ���,8;�������,9;δ��,10;�������,11;����ͨ��,12;������ͨ��,13;����ʧ��,14;�ɴ�ӡ�����,15";

//��Լ����״̬
const AnsiString TradestateContext = "��ʼ����,0;��ʼ�����,1;����,2;���۱���,3;���۴��,4;��������,5;��ͣ,6;\
    �����걨,7;�����걨����,8;�������걨,9;�����걨���,A;����,B";

//ת��
const AnsiString InOutAmtTypeContext = "ת��,0;ת��,1";

//�����״̬
const AnsiString InOutAmtStateContext = "�ɹ�,0;ʧ��,1;δ��δ����,2;δ���ֹ��Ѵ���,3;δ����,4";

//�ʽ���ˮ״̬
const AnsiString FundsSeqStateContext = "�ɹ�,0;ʧ��,1;δ��δ����,2;δ���ֹ��Ѵ���,3;δ����,4";

//�Ƿ���������
const AnsiString IsMidContext = "��,0;��,1";

//��������(�ʽ���ˮ)
const AnsiString FundsSeqType = "ת��,0;ת��,1;����,2;�ⶳ,3;����,4;�۳�,5;��ȡ,X";

//��������
const AnsiString FundsTypeContext = "��֤��,00;�����ʽ�,01;�����Ȩ�ִ���,02;���ڲ�����,03;���ӷ�,04;�˱���,05;���շ�,06;������,07;\
    ��������ˮ,08;����������,09;����������ΥԼ��,10;��Ա����ΥԼ��,11;��̷�,12;Ʒ������ˮ,13;��Ѻ�ǼǷ�,14;���ǼǷ�,15;\
    ��Աƽ��ΥԼ,16;��������,17;ƽ������,18;��������ʽ�,19;��Ϣ,20;��Ϣ˰,21;����ӯ��,22;������,23;���п���������,24;\
    �������������,25;����ִ���,26;˰����Ϣ,27;�����ʽ�,28;�������н��׷�������,29;�����������������,30;\
    ETF�ʽ�,31;������,32;�ֽ���,33";

//���仯����
const AnsiString TransTypeContext = "���,1;�������,2;��������޸�,3;���ճ����������,4;�������,5;�������,6;��Ѻ����Ȩ����,7;\
    ��Ѻ�����ʷ���,8;��Ѻע������Ȩ����,9;��Ѻע�������ʷ���,A;��裨�����,B;��裨���룩,C;���ɶ���,D;���ɶ���ⶳ,E;\
    �ֻ�����,F;�ֻ�����,G;��������,H;����,K;���ڽ�������,M;���ڽ�������,N;�������,O;������,P;���������͸֧,Q;�������������,R;\
    �Ǽ�������,S;�Ǽ�������,T;���ճ����������,U;ϵͳ�����������,V;\
    �Ϲ���ת�����Կͻ�,11;�Ϲ���ת�룩�Ի���˾,12;�Ϲ�ʧ�ܣ�ת�����Ի���˾,13;�Ϲ�ʧ�ܣ�ת�룩�Կͻ�,14;�깺��ת�����Կͻ�,15;\
    �깺��ת�룩�Ի���˾,16;��أ�ת�����Ի���˾,17;��أ�ת�룩�Կͻ�,18;Ӧ���ƽ�ETF�ǽ��׹���ת��,19;Ӧ���ƽ�ETF�ǽ��׹���ת��,20;�ǽ��׹���ת��,25;�ǽ��׹���ת��,26";

//֪ͨ������
const AnsiString RequizitionTypeContext = "׷��֪ͨ��,0;ǿƽ֪ͨ��,1";

//���յȼ�
const AnsiString RiskGradeContext = "��ɫǿƽ��,0;��ɫ׷����,1;��ɫ��ȫ��,2;��ɫ׷����,3";

//�г�״̬
const AnsiString MarketstateContext = "��ʼ����,0;��ʼ�����,1;����,2;����,3;��ͣ,4;����,5";

//�ļ��µ���ʶ
const AnsiString ReOrderChangeFlagContext = "�ѷ���,0;�Ѵ���,1";

//�������ѯ
const AnsiString ETFTradeType = "�Ϲ�,020;�깺,022;���,024";
                                             
//ȷ�ϱ�־
const AnsiString ETFStatus = "�Ϲ�����������,0;�Ϲ�����������,1;�Ϲ�����������,204;�Ϲ�ʧ��,205;�Ϲ��ɹ�,206;�Ϲ�ʧ��,207;\
                            �깺����������,3;�깺����������,4;�깺�ɹ�,104;�깺ʧ��,105;\
                            �������������,6;�������������,7;���ʧ��,8;���ʧ��,9;��سɹ�,304";

//������ȷ�ϱ�־��ѯ����
const AnsiString ETFQueryCond = "�Ϲ�����������,0;�Ϲ�ʧ��,1;�Ϲ��ɹ�,2;�깺����������,3;�깺�ɹ�,4;�깺ʧ��,5;�������������,6;���ʧ��,7;��سɹ�,8";

//����״̬
const AnsiString FundState = "���ɽ���,0;�ɽ���,1";

//�ͻ���״̬                  "δ��,0;���ذ󶨴�ȷ��,100;���ؽ���ȷ��,101;�󶨴�ȷ��,701;�������,702;����ʧ��,703;��󶨴�ȷ��,801;��������,802;�����ʧ��,803";
const AnsiString FundStatus = "δ��,0;������������,100;�������������,101;������������,701;�Ѱ�,702;δ��,703;�������������,801;δ��,802;�Ѱ�,803";

//�����Ƿ������깺/��ر�־
const AnsiString ETFSgShFlag = "�������깺/���,0;�깺����ؽ�����,1;�������깺,2;���������,3";

const char *VarietyTypeStr[] = {"1", "2", "3"};

const AnsiString Str_Ordsys_Type = "ֹ��,l;ֹӯ,s;������,c;�ƶ�ֹ��,ml;Ԥί��,p";
const AnsiString Str_Ordsys_Status = "��ǰ��Ч,o;�Ѿ�����,c;�û�����,d;ϵͳ����,s;����ʧ��,f;���ڷ���,h";
const AnsiString Str_Ordsys_Expire = "������Ч,d;������Ч,f";
const AnsiString Str_Ordsys_Condition = "���ڵ���,>=;С�ڵ���,<=;����,=;����,>;С��,<";
const AnsiString Str_Ordsys_OrdPriceType = "�Զ����,c;���¼�,l;��һ��,s;��һ��,b";

const AnsiString Str_LimitOptionCont = "������,0;����,1";

const AnsiString Str_OrderTypeContext = "��ͨί��,0;Ԥί��,1";

//---------------------------------------------------------------------------
void __fastcall SplitToStringList(const AnsiString &str, const AnsiString &spiltstr, TStringList *_list)
{
    _list->Clear();
    if (str.Length() == 0)
        return;
    AnsiString text(str);
    while(1)
    {
        int pos = text.Pos(spiltstr);
        if (pos == 0)
        {
            _list->Add(text.Trim());
            return;
        }
        else
        {
            _list->Add(text.SubString(1, pos - 1).Trim());
            text = text.SubString(pos + spiltstr.Length(), text.Length() - pos - spiltstr.Length()+1);
            if (text.IsEmpty())
                return;
        }
    }
}
//---------------------------------------------------------------------------
AnsiString __fastcall DoubleToAmountStr(double amount, int iSize)
{
    double d = 0.0;
    if (iSize > 0)
    {
      d = pow(10, -6-iSize) ;
    }
    AnsiString as = FloatToStrF(amount + d, ffNumber, 18, iSize);
    return as;
}
//---------------------------------------------------------------------------

#ifndef PERFORMANCETEST
void  __fastcall SetGridValByJson(TExtStringGrid *grid, Json::Value &obj_array)
{
    TIniFile *tini = new TIniFile(grid->IniFile.c_str());
    TStringList *slist = new TStringList;

    tini->ReadSectionValues(grid->IniSection.c_str(), slist);
    if(slist->Count <= 0)
    {
        delete slist;
        slist = NULL;
        delete tini;
        tini = NULL;
        return;
    }
    int count = 0;
    if(obj_array.isArray())
    {
        count = obj_array.size();
    }
    else
    {
        count = 1;
    }
    Json::Value &datasVal = obj_array;
    int row = -1;

    if (grid->RowCount <= grid->FixedRows+1)
    {
        if (grid->Cells[0][grid->FixedRows].IsEmpty())
        {
            row = grid->FixedRows;
        }
        else
        {
            row = grid->FixedRows+1;
        }
    }
    else
    {
        if (!grid->Cells[0][grid->FixedRows].IsEmpty())
        {
            row = grid->RowCount;
        }
        else
        {
            row = grid->FixedRows;
        }
    }
    /*
    if (grid->RowCount <= 2) //Ϊ��ʼҳ
    {
        if (grid->Cells[0][1].IsEmpty())
            row = 1;
        else
            row = 2;
    }
    else if ((grid->RowCount>2) && (!grid->Cells[0][1].IsEmpty()))
    {
       row = grid->RowCount; //��һҳ

       //��������һҳ
    //           grid->RowCount = grid->RowCount + recNum;
    }
    else
    {
        row = 1;
    }
    */
    
    for (int i = 0;i < count; i++)
    {
        Json::Value *pFields;
        if(obj_array.isArray())
        {
            pFields = &(datasVal[i]);
        }
        else
        {
            pFields = &(datasVal);
        }
        for (int k = 0; k < slist->Count; k++)
        {
            std::string skeyName = slist->Names[k].c_str();
            std::string tmpval = slist->Values[slist->Names[k]].c_str();
            int isNULL = skeyName.compare("");
            if (isNULL == 0)
               continue;
            TStringList *tmpvallst = new TStringList();
            SplitToStringList(AnsiString(tmpval.c_str()).c_str(), ";", tmpvallst);


            if ((*pFields)[skeyName].isString())
            {
                if (tmpvallst->Count > 3)  //˵���ж���������Ϣ
                {
                    char datatype[2];
                    char dataFormat[25];
                    memset(datatype, 0, 2);
                    memset(dataFormat, 0, 25);
                    sscanf(tmpvallst->Strings[3].c_str(), "%[^,],%[^,]", datatype, dataFormat);

                    if (datatype[0] == 'e') //��Ҫת��Ĵ���
                    {
                        AnsiString format = AnsiString(dataFormat);
                        EscapeStruct escape;
                        escape.Type = 1;
                        escape.Code = (*pFields)[skeyName].asCString();
                        GetEscape(format.c_str(), escape);
                        grid->Cells[k][row] = escape.Text;
                    }
                    else if (datatype[0] == 't')  //Ϊ������ʽ
                    {
                        int format = AnsiString(dataFormat).ToInt();
                        if (format == 8) //date
                            grid->Cells[k][row] = Date2Str(AnsiString((*pFields)[skeyName].asCString()).ToIntDef(0), '-').c_str();
                        else if (format == 6) //time
                            grid->Cells[k][row] = Time2Str(AnsiString((*pFields)[skeyName].asCString()).ToIntDef(0), ':').c_str();
                        else
                            grid->Cells[k][row] = AnsiString((*pFields)[skeyName].asCString()).ToIntDef(0);
                    }
                    else if (datatype[0] == 'f') //����������
                    {
                        AnsiString temp = AnsiString((*pFields)[skeyName].asCString());
                        if(temp == "")
                        {
                            grid->Cells[k][row] = "";
                        }
                        else
                        {
                            int precision = AnsiString(dataFormat).ToInt();
                            double amount = AnsiString(temp).ToDouble();
                            grid->Cells[k][row] = DoubleToAmountStr(amount, precision) + " ";
                        }
                    }
                    else if(datatype[0] == 'i') //��������
                    {
                        grid->Cells[k][row] = AnsiString((*pFields)[skeyName].asCString()) + " ";
                    }
                    else if(datatype[0] == '%')//�ٷֱ�����
                    {
                        AnsiString temp = AnsiString((*pFields)[skeyName].asCString());
                        if(temp == "")
                        {
                            grid->Cells[k][row] = "";
                        }
                        else
                        {
                            int precision = AnsiString(dataFormat).ToInt();
                            double amount = AnsiString(temp).ToDouble();
                            grid->Cells[k][row] = DoubleToAmountStr(amount * 100, precision) + " ";
                        }
                    }
                    else
                    {
                        grid->Cells[k][row] = (*pFields)[skeyName].asCString();
                    }
                }
                else
                    grid->Cells[k][row] = (*pFields)[skeyName].asCString();

            }
            else
            {
                if (tmpvallst->Count > 3) //˵���ж���������Ϣ
                {/*
                     char datatype[2];
                     char dataFormat[25];
                     memset(datatype,0,2);
                     memset(dataFormat,0,25);
                     sscanf(tmpvallst->Strings[3].c_str(),"%[^,],%[^,]",datatype,dataFormat);
                     /*if (datatype[0] == 'c')
                     {
                         AnsiString format = AnsiString(dataFormat);
                         AnsiString valStr = "";
                         skeyName=dataFormat;
                         if  (datasVal[i][skeyName].isInt())
                           valStr=GetEGridValue(format.c_str(),IntToStr(datasVal[i][skeyName].asInt()).c_str());
                         else if (datasVal[i][skeyName].isString())
                           valStr=GetEGridValue(format.c_str(),datasVal[i][skeyName].asString());
                         grid->Cells[k][row] = valStr;
                     } */

                }
                else
                {
                    grid->Cells[k][row] = AnsiString((*pFields)[skeyName].asString().c_str());
                }

            }
            delete tmpvallst;
            tmpvallst = NULL; 
        }
        row ++;
    }
    grid->RowCount = row <= 0 ?2 : row;

    delete slist;
    slist = NULL;
    delete tini;
    tini = NULL;
}
//---------------------------------------------------------------------------
#else
void  __fastcall SetGridValByJson(TExtStringGrid *grid, Json::Value &obj_array)
{
    TIniFile *tini = new TIniFile(grid->IniFile.c_str());
    TStringList *slist = new TStringList;

    tini->ReadSectionValues(grid->IniSection.c_str(), slist);
    if(slist->Count <= 0)
    {
        delete slist;
        slist = NULL;
        delete tini;
        tini = NULL;
        return;
    }
    int count = 0;
    if(obj_array.isArray())
    {
        count = obj_array.size();
    }
    else
    {
        count = 1;
    }
    Json::Value &datasVal = obj_array;
    int row = -1;

    if (grid->RowCount <= grid->FixedRows+1)
    {
        if (grid->Cells[0][grid->FixedRows].IsEmpty())
        {
            row = grid->FixedRows;
        }
        else
        {
            row = grid->FixedRows+1;
        }
    }
    else
    {
        if (!grid->Cells[0][grid->FixedRows].IsEmpty())
        {
            row = grid->RowCount;
        }
        else
        {
            row = grid->FixedRows;
        }
    }
    /*
    if (grid->RowCount <= 2) //Ϊ��ʼҳ
    {
        if (grid->Cells[0][1].IsEmpty())
            row = 1;
        else
            row = 2;
    }
    else if ((grid->RowCount>2) && (!grid->Cells[0][1].IsEmpty()))
    {
       row = grid->RowCount; //��һҳ

       //��������һҳ
    //           grid->RowCount = grid->RowCount + recNum;
    }
    else
    {
        row = 1;
    }
    */

    for (int i = 0;i < count; i++)
    {
        Json::Value *pFields;
        if(obj_array.isArray())
        {
            pFields = &(datasVal[i]);
        }
        else
        {
            pFields = &(datasVal);
        }
        for (int k = 0; k < slist->Count; k++)
        {
            std::string skeyName = slist->Names[k].c_str();
            std::string tmpval = slist->Values[slist->Names[k]].c_str();
            int isNULL = skeyName.compare("");
            if (isNULL == 0)
               continue;
            TStringList *tmpvallst = new TStringList();
            SplitToStringList(AnsiString(tmpval.c_str()).c_str(), ";", tmpvallst);


            if ((*pFields)[skeyName].isString())
            {
                if (tmpvallst->Count > 3)  //˵���ж���������Ϣ
                {
                    char datatype[2];
                    char dataFormat[25];
                    memset(datatype, 0, 2);
                    memset(dataFormat, 0, 25);
                    sscanf(tmpvallst->Strings[3].c_str(), "%[^,],%[^,]", datatype, dataFormat);

                    if (datatype[0] == 'e') //��Ҫת��Ĵ���
                    {
                        AnsiString format = AnsiString(dataFormat);
                        EscapeStruct escape;
                        escape.Type = 1;
                        escape.Code = (*pFields)[skeyName].asCString();
                        GetEscape(format.c_str(), escape);
                        grid->Cells[k][row] = escape.Text;
                    }
                    else
                    {
                        grid->Cells[k][row] = (*pFields)[skeyName].asCString();
                    }
                }
                else
                    grid->Cells[k][row] = (*pFields)[skeyName].asCString();

            }
            else if ((*pFields)[skeyName].isInt())  //������ֵ �� ����ֵ
            {
                if (tmpvallst->Count > 3)  //˵���ж���������Ϣ
                {
                    char datatype[2];
                    char dataFormat[25];
                    memset(datatype, 0, 2);
                    memset(dataFormat, 0, 25);
                    sscanf(tmpvallst->Strings[3].c_str(), "%[^,],%[^,]", datatype, dataFormat);

                    if (datatype[0] == 'e') //��Ҫת��Ĵ���
                    {
                        AnsiString format = AnsiString(dataFormat);
                        EscapeStruct escape;
                        escape.Type = 1;
                        escape.Code = (*pFields)[skeyName].asInt();
                        GetEscape(format.c_str(), escape);
                        grid->Cells[k][row] = escape.Text;
                    }
                    else if (datatype[0] == 't')  //Ϊ������ʽ
                    {
                        int format = AnsiString(dataFormat).ToInt();
                        if (format == 8) //date
                            grid->Cells[k][row] = Date2Str((*pFields)[skeyName].asInt(), '-').c_str();
                        else if (format == 6) //time
                            grid->Cells[k][row] = Time2Str((*pFields)[skeyName].asInt(), ':').c_str();
                        else
                            grid->Cells[k][row] = (*pFields)[skeyName].asInt();
                    }
                    else if(datatype[0] == 'i') //��������
                    {
                        grid->Cells[k][row] = AnsiString((*pFields)[skeyName].asInt()) + " ";
                    }
                    else if (datatype[0] == 'f') //Ϊ0�ĸ���������
                    {
                        AnsiString temp = AnsiString((*pFields)[skeyName].asInt());
                        if(temp == "")
                        {
                            grid->Cells[k][row] = "";
                        }
                        else
                        {
                            int precision = AnsiString(dataFormat).ToInt();
                            double amount = (*pFields)[skeyName].asInt();
                            grid->Cells[k][row] = DoubleToAmountStr(amount, precision) + " ";
                        }
                    }
                    else
                    {
                        grid->Cells[k][row] = (*pFields)[skeyName].asInt();
                    }
                }
                else
                {
                    grid->Cells[k][row] = AnsiString((*pFields)[skeyName].asInt());
                }

            }
            else if ((*pFields)[skeyName].isDouble())  //˫������
            {
                if (tmpvallst->Count > 3)  //˵���ж���������Ϣ
                {
                    char datatype[2];
                    char dataFormat[25];
                    memset(datatype, 0, 2);
                    memset(dataFormat, 0, 25);
                    sscanf(tmpvallst->Strings[3].c_str(), "%[^,],%[^,]", datatype, dataFormat);

                    if (datatype[0] == 'e') //��Ҫת��Ĵ���
                    {
                        AnsiString format = AnsiString(dataFormat);
                        EscapeStruct escape;
                        escape.Type = 1;
                        escape.Code = (*pFields)[skeyName].asDouble();
                        GetEscape(format.c_str(), escape);
                        grid->Cells[k][row] = escape.Text;
                    }
                    else if (datatype[0] == 'f') //����������
                    {
                        AnsiString temp = AnsiString((*pFields)[skeyName].asDouble());
                        if(temp == "")
                        {
                            grid->Cells[k][row] = "";
                        }
                        else
                        {
                            int precision = AnsiString(dataFormat).ToInt();
                            double amount = (*pFields)[skeyName].asDouble();
                            grid->Cells[k][row] = DoubleToAmountStr(amount, precision) + " ";
                        }
                    }
                    else if(datatype[0] == '%')//�ٷֱ�����
                    {
                        AnsiString temp = AnsiString((*pFields)[skeyName].asDouble());
                        if(temp == "")
                        {
                            grid->Cells[k][row] = "";
                        }
                        else
                        {
                            int precision = AnsiString(dataFormat).ToInt();
                            double amount = (*pFields)[skeyName].asDouble();
                            grid->Cells[k][row] = DoubleToAmountStr(amount * 100, precision) + " ";
                        }
                    }
                    else
                    {
                        grid->Cells[k][row] = (*pFields)[skeyName].asDouble();
                    }
                }
                else
                {
                    grid->Cells[k][row] = AnsiString((*pFields)[skeyName].asDouble());
                }
            }


            delete tmpvallst;
            tmpvallst = NULL;
        }
        row ++;
    }
    grid->RowCount = row <= 0 ?2 : row;

    delete slist;
    slist = NULL;
    delete tini;
    tini = NULL;
}
#endif
//---------------------------------------------------------------------------
AnsiString __fastcall Date2Str(int intime, char Separator)
{
    if(intime == 0)
    {
        return "-";
    }
    char szDateTime[11];
    memset(szDateTime, 0, 11);
    sprintf(szDateTime,"%4.4d%c%2.2d%c%2.2d", intime/10000, Separator, (intime/100)%100, Separator, intime%100);
    return AnsiString(szDateTime);
}
//---------------------------------------------------------------------------
AnsiString __fastcall Time2Str(int intime, char Separator)
{
    if(intime == 0)
    {
        return "-";
    }
    char szDateTime[9];
    memset(szDateTime, 0, 9);
    sprintf(szDateTime,"%2.2d%c%2.2d%c%2.2d", intime/10000, Separator, (intime/100)%100, Separator, intime%100);
    return AnsiString(szDateTime);
}
//---------------------------------------------------------------------------

void __fastcall SetControlFocus(TControl *control)
{
    try
    {
        TEdit* ctrl = (TEdit*)control;
        if(ctrl->CanFocus())
        {
        //if  (control->Visible && control->Enabled)
            ctrl->SetFocus();
        }
    }
    catch (...)
    {

    }
}
//--------------------------------------------------------------------------
AnsiString __fastcall InitContext(const AnsiString &context)
{
    //��������
    if(context == "TradeTypeContext")
    {
        return TradeTypeContext;
    }
    //�������� (�ļ��µ�ר���Ӽ�,ֻ��������combobox)
    else if(context == "TradeTypeReOrderContext")
    {
        return TradeTypeReOrderContext;
    }
    //�г�����
    else if(context == "MarkettypeContext")
    {
        return MarkettypeContext;
    }
    //�ɽ���״̬
    else if(context == "DeliveryStateContext")
    {
        return DeliveryStateContext;
    }
    //��������
    else if(context == "BuyOrSellContext")
    {
        return BuyOrSellContext;
    }
    //��������&���շ��� (��ѯ�����⴦��)
    else if(context == "BSJSContext")
    {
        return BSJSContext;
    }
    //���շ���
    else if(context == "JiaoOrShouContext")
    {
        return JiaoOrShouContext;
    }
    //ί��״̬
    else if(context == "OrderStateContext")
    {
        return OrderStateContext;
    }
    //��ƽ�ֱ�־
    else if(context == "OffsetFlagContext")
    {
        return OffsetFlagContext;
    }
    //��ƽ�ֱ�־ (��������ʾ)
    else if(context == "OffsetFlagContextR")
    {
        return OffsetFlagContextR;
    }
    //֧������
    else if(context == "PayDirectionContext")
    {
        return PayDirectionContext;
    }
    //�걨״̬(�������)
    else if(context == "ApplyStateContext")
    {
        return ApplyStateContext;
    }
    //�����״̬
    else if(context == "PickupApplyStateContext")
    {
        return PickupApplyStateContext;
    }
    //��Լ����״̬
    else if(context == "TradestateContext")
    {
        return TradestateContext;
    }
    //ת��
    else if(context == "InOutAmtTypeContext")
    {
        return InOutAmtTypeContext;
    }
    //�����״̬
    else if(context == "InOutAmtStateContext")
    {
        return InOutAmtStateContext;
    }
    else if(context == "FundsSeqStateContext")
    {
        return FundsSeqStateContext;
    }
    //�Ƿ���������
    else if(context == "IsMidContext")
    {
        return IsMidContext;
    }
    //��������(�ʽ���ˮ)
    else if(context == "FundsSeqType")
    {
        return FundsSeqType;
    }
    //��������
    else if(context == "FundsTypeContext")
    {
        return FundsTypeContext;
    }
    //���仯����
    else if(context == "TransTypeContext")
    {
        return TransTypeContext;
    }
    //֪ͨ������
    else if(context == "RequizitionTypeContext")
    {
        return RequizitionTypeContext;
    }
    //���յȼ�
    else if(context == "RiskGradeContext")
    {
        return RiskGradeContext;
    }
    //�г�״̬
    else if(context == "MarketstateContext")
    {
        return MarketstateContext;
    }
    //�ļ��µ���ʶ
    else if(context == "ReOrderChangeFlagContext")
    {
        return ReOrderChangeFlagContext;
    }
    else if(context == "ETFTradeType")
    {
        return ETFTradeType;
    }
    else if(context == "ETFStatus")
    {
        return ETFStatus;
    }
    else if(context == "FundState")
    {
        return FundState;
    }
    else if(context == "FundStatus")
    {
        return FundStatus;
    }
    else if (context == "ETFSgShFlag")
    {
        return ETFSgShFlag;
    }
    else if (context == "ETFQueryCond")
    {
        return ETFQueryCond;
    }
    else if (context == "Str_Ordsys_Type")
    {
        return Str_Ordsys_Type;
    }
    else if (context == "Str_Ordsys_Status")
    {
        return Str_Ordsys_Status;
    }
    else if (context == "Str_Ordsys_Expire")
    {
        return Str_Ordsys_Expire;
    }
    else if (context == "Str_Ordsys_Condition")
    {
        return Str_Ordsys_Condition;
    }
    else if (context == "Str_Ordsys_OrdPriceType")
    {
        return Str_Ordsys_OrdPriceType;
    }
    else if (context == "Str_LimitOptionCont")
    {
        return Str_LimitOptionCont;
    }
    else if (context == "Str_OrderTypeContext")
    {
        return Str_OrderTypeContext;
    }

    return "";
}
//--------------------------------------------------------------------------
void __fastcall GetEscape(const AnsiString &context, EscapeStruct &escape)
{
    TStringList *slist = new TStringList;
    AnsiString iniStr = "";

    iniStr = InitContext(context);

    if(!iniStr.IsEmpty())
    {
        SplitToStringList(iniStr.c_str(), ";", slist);
        if(slist->Count <= 0)
        {
            delete slist;
            return;
        }
        char code[30];
        char text[100];

        //��������ȡ���ֺʹ���
        if(escape.Type == 0)
        {
            memset(code, 0, 30);
            memset(text, 0, 100);
            sscanf(slist->Strings[escape.Index].c_str(),"%[^,],%[^,]", &text, &code);
            escape.Code = code;
            escape.Text = text;
        }
        //���ݴ���ȡ���ֺ�����
        else if(escape.Type == 1)
        {
            escape.Index = -1;
            escape.Text = "";
            for(int i = 0; i < slist->Count; i++)
            {
                memset(code, 0, 30);
                memset(text, 0, 100);
                sscanf(slist->Strings[i].c_str(),"%[^,],%[^,]", &text, &code);

                if(AnsiString(code) == escape.Code)
                {
                    escape.Text = text;
                    escape.Index = i;
                    break;
                }
            }
        }
        //��������ȡ���ź�����
        else if(escape.Type == 2)
        {
            escape.Index = -1;
            escape.Code = "";
            for(int i = 0; i < slist->Count; i++)
            {
                memset(code, 0, 30);
                memset(text, 0, 100);
                sscanf(slist->Strings[i].c_str(),"%[^,],%[^,]", &text, &code);

                if(AnsiString(text) == escape.Text)
                {
                    escape.Code = code;
                    escape.Index = i;
                    break;
                }
            }
        }
    }
    delete slist;
    slist = NULL;
    return;
}
//---------------------------------------------------------------------------
void __fastcall SetComboBoxText(const AnsiString &context, TComboBox *combo)
{
    TStringList *slist = new TStringList;
    AnsiString iniStr = "";

    iniStr = InitContext(context);

    if(!iniStr.IsEmpty())
    {
        SplitToStringList(iniStr.c_str(), ";", slist);
        if(slist->Count <= 0)
        {
            delete slist;
            return;
        }
        char code[30];
        char text[100];
        for(int i = 0; i < slist->Count; i++)
        {
            memset(code, 0, 30);
            memset(text, 0, 100);
            sscanf(slist->Strings[i].c_str(),"%[^,],%[^,]", &text, &code);
            combo->Items->Add(text);
        }
    }
    delete slist;
    slist = NULL;
    return;
}
//---------------------------------------------------------------------------
void __fastcall SetRadioGroupText(const AnsiString &context, TRadioGroup *rg)
{
    TStringList *slist = new TStringList;
    AnsiString iniStr = "";

    iniStr = InitContext(context);

    if(!iniStr.IsEmpty())
    {
        SplitToStringList(iniStr.c_str(), ";", slist);
        if(slist->Count <= 0)
        {
            delete slist;
            return;
        }
        char code[30];
        char text[100];
        for(int i = 0; i < slist->Count; i++)
        {
            memset(code, 0, 30);
            memset(text, 0, 100);
            sscanf(slist->Strings[i].c_str(),"%[^,],%[^,]", &text, &code);
            rg->Items->Add(text);
        }
    }
    delete slist;
    slist = NULL;
    return;
}
//---------------------------------------------------------------------------
void __fastcall GetExtradeDateReq()
{
    AUTD_HttpDealForm->ToBackHttp(hcGetExtradeDate);
    //AnsiString today = TDateTime::CurrentDateTime().FormatString("yyyymmdd");
    //g_iTodayOfTrading = today.ToInt();
}
//---------------------------------------------------------------------------
void __fastcall GetExtradeDateRecv(Json::Value &val)
{
#ifndef PERFORMANCETEST
    AnsiString strTemp = val["exTradeDate"].asCString();
#else
    AnsiString strTemp = val["exTradeDate"].asInt();
#endif
    g_iTodayOfTrading = strTemp.ToIntDef(INVALID_EXTRADEDATE);

    //����������
    if(AUTD_QueryCurrentForm != NULL)
    {
        if(g_iTodayOfTrading > 0 )
        {
            TDateTime dateTrade = EncodeDate(g_iTodayOfTrading / 10000, g_iTodayOfTrading % 10000 / 100, g_iTodayOfTrading % 100);
            AUTD_QueryCurrentForm->StatusBarQuery->Panels->Items[4]->Text = dateTrade.FormatString("yyyy-mm-dd");
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall GetExtradeDateFail()
{
    g_iTodayOfTrading = INVALID_EXTRADEDATE;
}
//--------------------------------------------------------------------------
void __fastcall GetExtradeDateReqInit()
{
    AUTD_HttpDealForm->ToBackHttp(hcGetExtradeDateInit);
}
//---------------------------------------------------------------------------
void __fastcall GetExtradeDateReqOpen()
{
    AUTD_HttpDealForm->ToBackHttp(hcGetExtradeDateOpen);
}
//---------------------------------------------------------------------------

/**���ɽ�����������*/
//�ʽ�
void __fastcall QueryCustFundsRecvInit(Json::Value &val)
{
    AnsiString listName = AnsiString("send") + HttpContext[hcQueryCustFundsInit].PackName;
    g_Funds.InitByJson(val[listName.c_str()]);
    g_bQueryCustFundsSucc = true;
    //SetEvent(m_eventQueryCustFunds);
    //g_valCustFundsInit = val;
}
//---------------------------------------------------------------------------
void __fastcall QueryCustFundsFailInit()
{
    g_bQueryCustFundsSucc = false;
    //SetEvent(m_eventQueryCustFunds);
}
//---------------------------------------------------------------------------
void __fastcall QueryCustFundsReqInit()
{
    Json::Value valQuery;
    ////�����¼�
    //ResetEvent(m_eventQueryCustFunds);
    AUTD_HttpDealForm->ToBackHttp(hcQueryCustFundsInit, valQuery);
}
//---------------------------------------------------------------------------
//���
void __fastcall QueryStorageRecvInit(Json::Value &val)
{
    int count = 0;
    AnsiString listName = AnsiString("send") + HttpContext[hcQueryStorageInit].PackName;
    count = val[listName.c_str()]["storageList"].size();
    if(count > 0)
    {
        for(int i = 0; i < count; ++i)
        {
            pubTranCode::Storage *storage = new pubTranCode::Storage;
            storage->InitByJson(val[listName.c_str()], i);

            g_vecStorageList.push_back(storage);
        }
        g_bQueryStorageSucc = true;
    }
    //SetEvent(m_eventQueryStorage);
    //g_valStorageInit = val;
}
//---------------------------------------------------------------------------
void __fastcall QueryStorageFailInit()
{
    g_bQueryStorageSucc = false;
}
//---------------------------------------------------------------------------
void __fastcall QueryStorageReqInit()
{
    Json::Value valQuery;

    std::vector<pubTranCode::Storage*>::iterator it;
    for(it = g_vecStorageList.begin(); it != g_vecStorageList.end(); ++it)
    {
        delete (*it);
    }
    g_vecStorageList.clear();

    valQuery["pageNo"] = 1;
    ////�����¼�
    //ResetEvent(m_eventQueryStorage);
    AUTD_HttpDealForm->ToBackHttp(hcQueryStorageInit, valQuery);
}
//---------------------------------------------------------------------------
//�ֲ�
void __fastcall QueryPositionRecvInit(Json::Value &val)
{
    int count = 0;
    AnsiString listName = AnsiString("send") + HttpContext[hcQueryPositionInit].PackName;
    count = val[listName.c_str()]["positionList"].size();
    if(count > 0)
    {
        for(int i = 0; i < count; ++i)
        {
            pubTranCode::Position *position = new pubTranCode::Position;
            position->InitByJson(val[listName.c_str()], i);
            
            g_vecPositionList.push_back(position);
        }
        g_bQueryPositionSucc = true;
    }
    //SetEvent(m_eventQueryPosition);
    //g_valPositionInit = val;
}
//---------------------------------------------------------------------------
void __fastcall QueryPositionFailInit()
{
    g_bQueryPositionSucc = false;
}
//---------------------------------------------------------------------------
void __fastcall QueryPositionReqInit()
{
    Json::Value valQuery;

    std::vector<pubTranCode::Position*>::iterator it;
    for(it = g_vecPositionList.begin(); it != g_vecPositionList.end(); ++it)
    {
        delete (*it);
    }
    g_vecPositionList.clear();

    //ҳ��
    valQuery["pageNo"] = 1;
    ////�����¼�
    //ResetEvent(m_eventQueryPosition);
    AUTD_HttpDealForm->ToBackHttp(hcQueryPositionInit, valQuery);
}
//---------------------------------------------------------------------------
//���׷���
//---------------------------------------------------------------------------
void __fastcall QueryTradeFeeRateRecvInit(Json::Value &val)
{
    AnsiString strTemp = "";
    int count = 0;
    AnsiString listName = AnsiString("send") + HttpContext[hcQueryTradeFeeRateInit].PackName;
    count = val[listName.c_str()]["tradeFeeRateList"].size();
    if(count > 0)
    {
        for(int i = 0; i < count; ++i)
        {
            pubTranCode::TradeFeeRate *tradeFeeRate = new pubTranCode::TradeFeeRate;
            tradeFeeRate->InitByJson(val[listName.c_str()], i);
            g_vecTradeFeeRateList.push_back(tradeFeeRate);
        }
        g_bQueryTradeFeeRateSucc = true;
    }
    //SetEvent(m_eventQueryTradeFeeRate);
}
//---------------------------------------------------------------------------
//���׷��ʻ�ȡ2
void __fastcall UpdateTradeFeeRateByAs(AnsiString AStr)
{
    int ipos = AStr.Pos("|");
    AnsiString asTitle = AStr.SubString(1, ipos - 1);
    int Handle = 0;
    int Count = 0;
    sscanf(asTitle.c_str(), "%d:%d", &Handle, &Count);
    if ((-1 == Handle) || (0 == Count))
    {
        return;
    }

    int deliveryState = 0;
    double orderSpread = 0;     //��������ί�м۲�
    double Threshold = 0;       //��Ʒ��ֵ
    char cFreezeSort[21];       //����Ʒ�ֶ���˳��
    char cIsOpenETF[4];
    memset(cFreezeSort,0,21);
    memset(cIsOpenETF,0,4);

    AnsiString asFormat= (AnsiString)"%[^,],%[^,],\
                                        %lf,%lf,%lf,%lf,\
                                        %lf,%lf,%lf,%lf,\
                                        %lf,%lf,\
                                        %lf,%lf,\
                                        %lf,%lf,%lf,%lf,%lf,%lf,%lf,\
                                        %lf,%lf,%lf,%lf,%lf,%lf,%[^,],%[^,],%d";

	AnsiString  asTemp = AStr.SubString(ipos + 1, AStr.Length() - ipos);
	TStringList *tmpList1= new TStringList();

    asTemp = StringReplace(asTemp, " ", "", TReplaceFlags() << rfReplaceAll);
	tmpList1->Delimiter = ';';
	tmpList1->DelimitedText = asTemp;
    for (int i = 0 ;i < tmpList1->Count; i++)
	{
        if (!tmpList1->Strings[i].IsEmpty())
        {
            try
            {
                pubTranCode::TradeFeeRate tradeFeeRateTemp;
                sscanf(tmpList1->Strings[i].c_str(), asFormat.c_str(),
                    &tradeFeeRateTemp.instId,
                    &tradeFeeRateTemp.instName,
                    &tradeFeeRateTemp.buyOpenGoldFee,
                    &tradeFeeRateTemp.sellOpenGoldFee,
                    &tradeFeeRateTemp.buyOffsetGoldFee,
                    &tradeFeeRateTemp.sellOffsetGoldFee,
                    &tradeFeeRateTemp.buyOpenBankFee,
                    &tradeFeeRateTemp.sellOpenBankFee,
                    &tradeFeeRateTemp.buyOffsetBankFee,
                    &tradeFeeRateTemp.sellOffsetBankFee,
                    &tradeFeeRateTemp.buyBankMargin,
                    &tradeFeeRateTemp.sellBankMargin,
                    &tradeFeeRateTemp.buyGoldMargin,
                    &tradeFeeRateTemp.sellGoldMargin,
                    &tradeFeeRateTemp.deliveryFeePoint,
                    &tradeFeeRateTemp.pickStoreFee,
                    &tradeFeeRateTemp.payDayFeePoint,
                    &tradeFeeRateTemp.transportFee,
                    &tradeFeeRateTemp.breachFeePoint,
                    &tradeFeeRateTemp.readyPriceRise,
                    &tradeFeeRateTemp.readySpill,
                    &tradeFeeRateTemp.offsetSpotBuyGoldFee,
                    &tradeFeeRateTemp.offsetSpotSellGoldFee,
                    &tradeFeeRateTemp.offsetSpotBuyBankFee,
                    &tradeFeeRateTemp.offsetSpotSellBankFee,
                    &orderSpread,
                    &Threshold,
                    cFreezeSort,
                    cIsOpenETF,
                    &deliveryState
                    );

                int size = g_vecTradeFeeRateList.size();
                int nCnt = 0;
                for(nCnt = 0; nCnt < size; ++nCnt)
                {
                    if(strcmp(g_vecTradeFeeRateList[nCnt]->instId, tradeFeeRateTemp.instId) == 0)
                    {
                        memcpy(g_vecTradeFeeRateList[nCnt], &tradeFeeRateTemp, sizeof(pubTranCode::TradeFeeRate));
                        break;
                    }
                }
                if(nCnt == size)
                {
                    pubTranCode::TradeFeeRate *tradeFeeRate = new pubTranCode::TradeFeeRate;
                    memcpy(tradeFeeRate, &tradeFeeRateTemp, sizeof(pubTranCode::TradeFeeRate));
                    g_vecTradeFeeRateList.push_back(tradeFeeRate);
                }

                AnsiString strInstId = tradeFeeRateTemp.instId;
                TPriceItem *item = g_pPriceList->GetItemByInstid(strInstId);
                if (item)
                {
                    String oldEtfOpen = item->isOpenETF;
                    
                    item->orderSpread = orderSpread;
                    item->Threshold = Threshold;
                    item->freezeSort = cFreezeSort;
                    item->isOpenETF = cIsOpenETF;
                    item->deliveryState = deliveryState;

                    if (AUTD_QueryCurrentForm)
                    {
                        AUTD_QueryCurrentForm->SetDataDeryInit(AUTD_QueryCurrentForm->FsInstId2, false);
                        AUTD_QueryCurrentForm->SetDataMidInit(AUTD_QueryCurrentForm->FsInstId3, false);
                    }
                        
                    if (oldEtfOpen != item->isOpenETF)
                    {
                        if (AUTD_QueryCurrentForm)
                        {
                            AUTD_QueryCurrentForm->RefreshEtfInstId();
                        }
                    }
                }
            }
            catch(...)
            {

            }
        }
	}
	delete tmpList1;
    tmpList1 = NULL;
}
//---------------------------------------------------------------------------
void __fastcall QueryTradeFeeRateFailInit()
{
    g_bQueryTradeFeeRateSucc = false;
}
//---------------------------------------------------------------------------
void __fastcall QueryTradeFeeRateReqInit()
{
    //���׷��ʲ�ѯ
    Json::Value valQuery;
    std::vector<pubTranCode::TradeFeeRate*>::iterator it;
    for(it = g_vecTradeFeeRateList.begin(); it != g_vecTradeFeeRateList.end(); ++it)
    {
        delete *it;
    }
    g_vecTradeFeeRateList.clear();

    //ҳ��
    valQuery["pageNo"] = 1;
    ////�����¼�
    //ResetEvent(m_eventQueryTradeFeeRate);
    AUTD_HttpDealForm->ToBackHttp(hcQueryTradeFeeRateInit, valQuery);
}
//---------------------------------------------------------------------------
//ϵͳ������ȡ
void __fastcall QuerySysParamReq(int pageNo)
{
    Json::Value valQuery;
    valQuery["packname"] = HttpContext[hcQuerySysParam].PackName;
    valQuery["topBank"] = g_loginInfo.topBank;
    valQuery["pageNo"] = pageNo;
    AUTD_HttpDealForm->ToBackHttp(hcQuerySysParam, valQuery);
}
//---------------------------------------------------------------------------
void __fastcall QuerySysParamFail()
{
    g_bQuerySysParamSucc = false;
}
//---------------------------------------------------------------------------
void __fastcall QuerySysParamRecv(Json::Value &val)
{
    AnsiString strTemp = "";
    int count = 0;
    AnsiString listName = AnsiString("send") + HttpContext[hcQuerySysParam].PackName;
    count = val[listName.c_str()]["sysParamList"].size();
    if(count > 0)
    {
        for(int i = 0; i < count; ++i)
        {
            pubTranCode::SysParam *sysParam = new pubTranCode::SysParam;
            sysParam->InitByJson(val[listName.c_str()], i);

            g_mapSysParamList.insert(pair<String, pubTranCode::SysParam *>(String(sysParam->name), sysParam));

            if(strcmp(sysParam->name, "Risk_Warn") == 0)
            {
                g_dRisk_Warn = double(AnsiString(sysParam->value).ToDouble() / 100);
            }
            else if(strcmp(sysParam->name, "Risk_TradeForce") == 0)
            {
                g_dRisk_TradeForce = double(AnsiString(sysParam->value).ToDouble() / 100);
            }
            else if(strcmp(sysParam->name, "MarginRatio") == 0)
            {
                g_dMarginRadio = AnsiString(sysParam->value).ToDouble() / 100;
            }
            else if(strcmp(sysParam->name, "InfoUpdateTime") == 0)
            {
                g_iInfoUpdateTime = AnsiString(sysParam->value).ToDouble();        //��Ϣ��������������λ:��
            }
            else if(strcmp(sysParam->name, "ZSZYLimit") == 0)  //�ͻ���ֹ��ֹӯ������ñ�������
            {
                g_iZSZYLimit = AnsiString(sysParam->value).ToInt();
            }
            else if(strcmp(sysParam->name, "TJDLimit") == 0)  //�ͻ���������������ñ�������
            {
                g_iTJDLimit = AnsiString(sysParam->value).ToInt();
            }
            else if(strcmp(sysParam->name, "TJDPriceLimit") == 0)  //�ͻ���������������������
            {
                g_iTJDPriceLimit = AnsiString(sysParam->value).ToInt();
            }
            else if (strcmp(sysParam->name, "IsOpenETF") == 0)  //ETF�����Ƿ���
            {
                g_iIsOpenETF = AnsiString(sysParam->value).ToInt();
            }
            else if (strcmp(sysParam->name, "YDZSLimit") == 0)
            {
                g_iTrailingStopLimit = AnsiString(sysParam->value).ToInt();
            }
            else if (strcmp(sysParam->name, "YDZSMaxPointLimit") == 0)
            {
                g_iTSPointUpLimit = AnsiString(sysParam->value).ToInt();
            }
            else if (strcmp(sysParam->name, "YDZSMinPointLimit") == 0)
            {
                g_iTSPointDownLimit = AnsiString(sysParam->value).ToInt();
            }
        }
        g_bQuerySysParamSucc = true;
    }
    else
    {
        g_bQuerySysParamSucc = false;
    }
    if(AnsiString(val[listName.c_str()]["endFlag"].asCString()).ToIntDef(0) == 1) //���к�����
    {
#ifndef PERFORMANCETEST
        int pageNo = AnsiString(val[listName.c_str()]["pageNo"].asCString()).ToIntDef(0);
#else
        int pageNo = val[listName.c_str()]["pageNo"].asInt();
#endif
        SysParamListPageNo = pageNo;
        SysParamListNextPage = true;
    }
    else
    {
        SysParamListNextPage = false;
    }

    //��֤��Ϣ���������� > 0
    if(g_iInfoUpdateTime <= 0)
    {
        g_iInfoUpdateTime = 10;
    }
}
//---------------------------------------------------------------------------

void __fastcall UpdateSysParamsByAs(AnsiString AStr)
{
    int ipos = AStr.Pos("|");
    AnsiString asTitle = AStr.SubString(1, ipos - 1);
    int Handle = 0;
    int Count = 0;
    sscanf(asTitle.c_str(), "%d:%d", &Handle, &Count);
    if ((-1 == Handle) || (0 == Count))
    {
        return;
    }

    AnsiString asFormat = (AnsiString)"%[^,],%[^,],%[^,]" ;

	AnsiString  asTemp = AStr.SubString(ipos + 1, AStr.Length() - ipos);
	TStringList *tmpList1= new TStringList();

    asTemp= StringReplace(asTemp, " ", "", TReplaceFlags() << rfReplaceAll);
	tmpList1->Delimiter = ';';
	tmpList1->DelimitedText = asTemp;
    for (int i = 0 ;i < tmpList1->Count; i++)
	{
        if (!tmpList1->Strings[i].IsEmpty())
        {
            try
            {
                pubTranCode::SysParam param;
                sscanf(tmpList1->Strings[i].c_str(), asFormat.c_str(),
                    &param.name,
                    &param.value//,
                    //&param.memo
                    );

                if(strcmp(param.name, "TouchAndDelegateSpread") == 0)
                {
                    AnsiString temp00 = tmpList1->Strings[i];
                    AnsiString temp01 = "";
                    int iTemppos = temp00.Pos(",");
                    temp00 = temp00.SubString(iTemppos + 1, temp00.Length() - iTemppos);
                    iTemppos = temp00.Pos(",");
                    temp01 = temp00.SubString(1, iTemppos - 1);
                    temp00 = temp00.SubString(iTemppos + 1, temp00.Length() - iTemppos);
                    iTemppos = temp00.Pos(",");
                    temp00 = temp00.SubString(1, iTemppos - 1);
                    temp00 = temp01 + "," + temp00;
                    memcpy(&param.value, temp00.c_str(), temp00.Length());
                    param.value[temp00.Length()] = '\0';
                }
                map<String, pubTranCode::SysParam*>::iterator m_Iter_1;
                m_Iter_1 = g_mapSysParamList.find(String(param.name));
    
                if (m_Iter_1 != g_mapSysParamList.end())
                {
                    memcpy(m_Iter_1->second, &param, sizeof(pubTranCode::SysParam));


                }
                else
                {
                     pubTranCode::SysParam *sysParam = new pubTranCode::SysParam;
                     memcpy(sysParam, &param, sizeof(pubTranCode::SysParam));
                     g_mapSysParamList.insert(pair<String, pubTranCode::SysParam *>(String(sysParam->name), sysParam));
                }

                if(strcmp(param.name, "Risk_Warn") == 0)
                {
                    g_dRisk_Warn = double(AnsiString(param.value).ToDouble() / 100);
                }
                else if(strcmp(param.name, "Risk_TradeForce") == 0)
                {
                    g_dRisk_TradeForce = double(AnsiString(param.value).ToDouble() / 100);
                }
                else if(strcmp(param.name, "MarginRatio") == 0)
                {
                    g_dMarginRadio = AnsiString(param.value).ToDouble() / 100;
                }
                else if(strcmp(param.name, "InfoUpdateTime") == 0)
                {
                    g_iInfoUpdateTime = AnsiString(param.value).ToDouble();        //��Ϣ��������������λ:��
                }
                else if(strcmp(param.name, "ZSZYLimit") == 0)  //�ͻ���ֹ��ֹӯ������ñ�������
                {
                    g_iZSZYLimit = AnsiString(param.value).ToInt();
                }
                else if(strcmp(param.name, "TJDLimit") == 0)  //�ͻ���������������ñ�������
                {
                    g_iTJDLimit = AnsiString(param.value).ToInt();
                }
                else if(strcmp(param.name, "TJDPriceLimit") == 0)  //�ͻ���������������������
                {
                    g_iTJDPriceLimit = AnsiString(param.value).ToInt();
                }
                else if (strcmp(param.name, "IsOpenETF") == 0)  //ETF�����Ƿ���
                {
                    g_iIsOpenETF = AnsiString(param.value).ToInt();
                }
                else if (strcmp(param.name, "YDZSLimit") == 0)
                {
                    g_iTrailingStopLimit = AnsiString(param.value).ToInt();
                }
                else if (strcmp(param.name, "YDZSMaxPointLimit") == 0)
                {
                    g_iTSPointUpLimit = AnsiString(param.value).ToInt();
                }
                else if (strcmp(param.name, "YDZSMinPointLimit") == 0)
                {
                    g_iTSPointDownLimit = AnsiString(param.value).ToInt();
                }
                else if (strcmp(param.name, "UpdateETFList") == 0)  //ETF�����嵥���֪ͨ
                {
                    if (AnsiString(param.value) == "1")
                    {
                        if (AUTD_QueryCurrentForm)
                        {
                            AUTD_QueryCurrentForm->btnETFQuaQryClick(NULL);
                        }
                    }
                }
            }
            catch(...)
            {
                AnsiString strMsg = "����ϵͳ��������,������Ϣ��" + asTemp;
                PackagePublic->ShowWarning(strMsg);
            }
        }
	}
	delete tmpList1;
    tmpList1 = NULL;
    
    //��֤��Ϣ���������� > 0
    if(g_iInfoUpdateTime <= 0)
    {
        g_iInfoUpdateTime = 10;
    }
    //�������ö�ʱ��
    AUTD_MainForm->SetQueryDoQueryReqInterval();
}
//---------------------------------------------------------------------------
AnsiString __fastcall DoInOutAmtReturn(Json::Value &val)
{
    AnsiString packName = AnsiString("send") + HttpContext[hcDoInOutAmt].PackName;
#ifndef PERFORMANCETEST
    AnsiString tranTypeCode = val[packName.c_str()]["tranType"].asCString();
    AnsiString tempStr = val[packName.c_str()]["amount"].asCString();
    double amount = tempStr.ToDouble();
#else
    AnsiString tranTypeCode = val[packName.c_str()]["tranType"].asInt();
    double amount = val[packName.c_str()]["amount"].asDouble();
#endif
    //AnsiString serialNo = val[packName.c_str()]["serialNo"].asCString();

    EscapeStruct escape;
    escape.Type = 1;
    escape.Code = tranTypeCode;
    GetEscape("InOutAmtTypeContext", escape);
    AnsiString tranType = escape.Text;

    return AnsiString("�����ת�˳ɹ�") + tranType + "�ʽ�\n" + FloatToStrF(amount, ffNumber, 18, 2);

}
//---------------------------------------------------------------------------
AnsiString __fastcall DoPickupApplyReturn(Json::Value &val)
{
    AnsiString packName = AnsiString("send") + HttpContext[hcDoPickupApply].PackName;
    AnsiString localPickupNo = val[packName.c_str()]["localPickupNo"].asCString();
    return AnsiString("�������ɹ���\n") + "�������ţ�" + localPickupNo;
}
//---------------------------------------------------------------------------
//�ͻ����շ��ղ�ѯ
void __fastcall QueryYesterdayRiskRecv(Json::Value &val)
{
    AnsiString strTemp = "";
    int count = 0;
    AnsiString listName = AnsiString("send") + HttpContext[hcQueryRisk].PackName;
    count = val[listName.c_str()]["riskList"].size();
    if(count > 0)
    {
        /*
        for(int i = 0; i < count; ++i)
        {
            pubTranCode::YesterdayRisk *yesterdayRisk = new pubTranCode::YesterdayRisk;
            yesterdayRisk->InitByJson(val[listName.c_str()], i);
            g_vecYesterdayRiskList.push_back(yesterdayRisk);
        }
        */
        g_YesterdayRisk.InitByJson(val[listName.c_str()], 0);
        g_bQueryYesterdayRiskSucc = true;
    }
    else
    {
        //memset(&g_YesterdayRisk, 0, sizeof(g_YesterdayRisk));
        g_YesterdayRisk.availAmt = 0.0;
        g_YesterdayRisk.amount = 0.0;
        g_YesterdayRisk.margin = 0.0;
        g_YesterdayRisk.risk = 0.0;
        g_YesterdayRisk.exMargin = 0.0;
        g_YesterdayRisk.tradeDate = 0;
        g_YesterdayRisk.warnRisk = 0.0;
        g_YesterdayRisk.forceRisk = 0.0;
        g_bQueryYesterdayRiskSucc = true;
        //g_bQueryYesterdayRiskSucc = false;
    }
    /*
    if(AnsiString(val[listName.c_str()]["endFlag"].asCString()).ToIntDef(0) == 1) //���к�����
    {
        int pageNo = AnsiString(val[listName.c_str()]["pageNo"].asCString()).ToIntDef(0);
        YesterdayRiskListPageNo = pageNo;
        YesterdayRiskListNextPage = true;
    }
    else
    {
        YesterdayRiskListNextPage = false;
    }
    */
}
//---------------------------------------------------------------------------
void __fastcall QueryYesterdayRiskReqOpen(int pageNo)
{
    if(pageNo == 1)
    {
        valQueryYesterdayRisk.clear();
        /*
        //��ѯ��ʼ����
        valQueryYesterdayRisk["startDate"] = date.FormatString("yyyymmdd").ToInt();
        //��ѯ��������
        valQueryYesterdayRisk["endDate"] = date.FormatString("yyyymmdd").ToInt();
        */
        //��ѯ��ʼ����
        valQueryYesterdayRisk["startDate"] = "20120201";
        //��ѯ��������
        valQueryYesterdayRisk["endDate"] = "20120228";

    }
    //ҳ��
    valQueryYesterdayRisk["pageNo"] = pageNo;
    AUTD_HttpDealForm->ToBackHttp(hcQueryRiskOpen, valQueryYesterdayRisk);
}
//---------------------------------------------------------------------------
void __fastcall QueryYesterdayRiskReq(int pageNo)
{
    if(pageNo == 1)
    {
        /*
        std::vector<pubTranCode::YesterdayRisk*>::iterator it;
        for(it = g_vecYesterdayRiskList.begin(); it != g_vecYesterdayRiskList.end(); ++it)
        {
            delete (*it);
        }
        g_vecYesterdayRiskList.clear();
          */
        /*
        // ��ȡ����������
        GetExtradeDateReq();
        if(INVALID_EXTRADEDATE == g_iTodayOfTrading)
            return;
        
        TDateTime date = EncodeDate(g_iTodayOfTrading / 10000, g_iTodayOfTrading % 10000 / 100, g_iTodayOfTrading % 100);
        date = date - 1;
        */
        valQueryYesterdayRisk.clear();
        /*
        //��ѯ��ʼ����
        valQueryYesterdayRisk["startDate"] = date.FormatString("yyyymmdd").ToInt();
        //��ѯ��������
        valQueryYesterdayRisk["endDate"] = date.FormatString("yyyymmdd").ToInt();
        */
        //��ѯ��ʼ����
        valQueryYesterdayRisk["startDate"] = "20120201";
        //��ѯ��������
        valQueryYesterdayRisk["endDate"] = "20120228";

    }
    //ҳ��
    valQueryYesterdayRisk["pageNo"] = pageNo;
    AUTD_HttpDealForm->ToBackHttp(hcQueryRisk, valQueryYesterdayRisk);
}
//---------------------------------------------------------------------------
void __fastcall QueryYesterdayRiskFail()
{
    g_bQueryYesterdayRiskSucc = false;
}
//---------------------------------------------------------------------------
void __fastcall GetAllYesterdayRiskRecords()
{
    //��ѯϵͳ��Ϣ
    YesterdayRiskListPageNo = 1;
    QueryYesterdayRiskReq(YesterdayRiskListPageNo);
    /*
    while(YesterdayRiskListNextPage)
    {
        QueryYesterdayRiskReq(YesterdayRiskListPageNo);

    }
    */
}
//---------------------------------------------------------------------------
//��½ʱһ���Գ�ʼ�����ݲ�ѯ
void __fastcall QueryLoginInitRecv(Json::Value &val)
{
    AnsiString listName = AnsiString("send") + HttpContext[hcLoginInitialization].PackName;
    //�ʽ�
    {
        g_Funds.InitByJson(val[listName.c_str()]);
        g_bQueryCustFundsSucc = true;
        //SetEvent(m_eventQueryCustFunds);
        //g_valCustFundsInit = val;
    }

    //���
    {
        int count = 0;
        count = val[listName.c_str()]["storageList"].size();
        if(count > 0)
        {
            for(int i = 0; i < count; ++i)
            {
                pubTranCode::Storage *storage = new pubTranCode::Storage;
                storage->InitByJson(val[listName.c_str()], i);

                g_vecStorageList.push_back(storage);
            }
            g_bQueryStorageSucc = true;
        }
        //SetEvent(m_eventQueryStorage);
        //g_valStorageInit = val;
    }

    //�ֲ�
    {
        int count = 0;
        count = val[listName.c_str()]["positionList"].size();
        if(count > 0)
        {
            for(int i = 0; i < count; ++i)
            {
                pubTranCode::Position *position = new pubTranCode::Position;
                position->InitByJson(val[listName.c_str()], i);

                g_vecPositionList.push_back(position);
            }
            g_bQueryPositionSucc = true;
        }
        //SetEvent(m_eventQueryPosition);
        //g_valPositionInit = val;
    }

    //���׷���
    {
        AnsiString strTemp = "";
        int count = 0;
        count = val[listName.c_str()]["tradeFeeRateList"].size();
        if(count > 0)
        {
            for(int i = 0; i < count; ++i)
            {
                pubTranCode::TradeFeeRate *tradeFeeRate = new pubTranCode::TradeFeeRate;
                tradeFeeRate->InitByJson(val[listName.c_str()], i);
                g_vecTradeFeeRateList.push_back(tradeFeeRate);
            }
            g_bQueryTradeFeeRateSucc = true;
        }
    }

    //ϵͳ����
    {
        AnsiString strTemp = "";
        int count = 0;
        count = val[listName.c_str()]["sysParamList"].size();
        if(count > 0)
        {
            for(int i = 0; i < count; ++i)
            {
                pubTranCode::SysParam *sysParam = new pubTranCode::SysParam;
                sysParam->InitByJson(val[listName.c_str()], i);

                g_mapSysParamList.insert(pair<String, pubTranCode::SysParam *>(String(sysParam->name), sysParam));

                if(strcmp(sysParam->name, "Risk_Warn") == 0)
                {
                    g_dRisk_Warn = double(AnsiString(sysParam->value).ToDouble() / 100);
                }
                else if(strcmp(sysParam->name, "Risk_TradeForce") == 0)
                {
                    g_dRisk_TradeForce = double(AnsiString(sysParam->value).ToDouble() / 100);
                }
                else if(strcmp(sysParam->name, "MarginRatio") == 0)
                {
                    g_dMarginRadio = AnsiString(sysParam->value).ToDouble() / 100;
                }
                else if(strcmp(sysParam->name, "InfoUpdateTime") == 0)
                {
                    g_iInfoUpdateTime = AnsiString(sysParam->value).ToDouble();        //��Ϣ��������������λ:��
                }
                else if(strcmp(sysParam->name, "ZSZYLimit") == 0)  //�ͻ���ֹ��ֹӯ������ñ�������
                {
                    g_iZSZYLimit = AnsiString(sysParam->value).ToInt();
                }
                else if(strcmp(sysParam->name, "TJDLimit") == 0)  //�ͻ���������������ñ�������
                {
                    g_iTJDLimit = AnsiString(sysParam->value).ToInt();
                }
                else if(strcmp(sysParam->name, "TJDPriceLimit") == 0)  //�ͻ���������������������
                {
                    g_iTJDPriceLimit = AnsiString(sysParam->value).ToInt();
                }
                else if (strcmp(sysParam->name, "YDZSLimit") == 0)
                {
                    g_iTrailingStopLimit = AnsiString(sysParam->value).ToInt();
                }
                else if (strcmp(sysParam->name, "YDZSMaxPointLimit") == 0)
                {
                    g_iTSPointUpLimit = AnsiString(sysParam->value).ToInt();
                }
                else if (strcmp(sysParam->name, "YDZSMinPointLimit") == 0)
                {
                    g_iTSPointDownLimit = AnsiString(sysParam->value).ToInt();
                }
                else if (strcmp(sysParam->name, "IsOpenETF") == 0)  //ETF�����Ƿ���
                {
                    g_iIsOpenETF = AnsiString(sysParam->value).ToInt();
                }
            }
            g_bQuerySysParamSucc = true;
        }
        else
        {
            g_bQuerySysParamSucc = false;
        }
        /**
        if(AnsiString(val[listName.c_str()]["endFlag"].asCString()).ToIntDef(0) == 1) //?1��Do��D?�㨹
        {
            int pageNo = AnsiString(val[listName.c_str()]["pageNo"].asCString()).ToIntDef(0);
            SysParamListPageNo = pageNo;
            SysParamListNextPage = true;
        }
        else
        {
            SysParamListNextPage = false;
        }
        */
        //��֤��Ϣ���������� > 0
        if(g_iInfoUpdateTime <= 0)
        {
            g_iInfoUpdateTime = 10;
        }
    }

    //�ͻ����շ���
    {
        AnsiString strTemp = "";
        int count = 0;
        count = val[listName.c_str()]["riskList"].size();
        if(count > 0)
        {
            /*
            for(int i = 0; i < count; ++i)
            {
                pubTranCode::YesterdayRisk *yesterdayRisk = new pubTranCode::YesterdayRisk;
                yesterdayRisk->InitByJson(val[listName.c_str()], i);
                g_vecYesterdayRiskList.push_back(yesterdayRisk);
            }
            */
            g_YesterdayRisk.InitByJson(val[listName.c_str()], 0);
            g_bQueryYesterdayRiskSucc = true;
        }
        else
        {
            //memset(&g_YesterdayRisk, 0, sizeof(g_YesterdayRisk));
            g_YesterdayRisk.availAmt = 0.0;
            g_YesterdayRisk.amount = 0.0;
            g_YesterdayRisk.margin = 0.0;
            g_YesterdayRisk.risk = 0.0;
            g_YesterdayRisk.exMargin = 0.0;
            g_YesterdayRisk.tradeDate = 0;
            g_YesterdayRisk.warnRisk = 0.0;
            g_YesterdayRisk.forceRisk = 0.0;
            g_bQueryYesterdayRiskSucc = true;
            //g_bQueryYesterdayRiskSucc = false;
        }
        /*
        if(AnsiString(val[listName.c_str()]["endFlag"].asCString()).ToIntDef(0) == 1) //?1��Do��D?�㨹
        {
            int pageNo = AnsiString(val[listName.c_str()]["pageNo"].asCString()).ToIntDef(0);
            YesterdayRiskListPageNo = pageNo;
            YesterdayRiskListNextPage = true;
        }
        else
        {
            YesterdayRiskListNextPage = false;
        }
        */
    }

    //����������
    {
#ifndef PERFORMANCETEST
        AnsiString strTemp = val[listName.c_str()]["exTradeDate"].asCString();
#else
        AnsiString strTemp = val[listName.c_str()]["exTradeDate"].asInt();
#endif

        g_iTodayOfTrading = strTemp.ToIntDef(INVALID_EXTRADEDATE);

        //����������
        if(AUTD_QueryCurrentForm != NULL)
        {
            if(g_iTodayOfTrading > 0 )
            {
                TDateTime dateTrade = EncodeDate(g_iTodayOfTrading / 10000, g_iTodayOfTrading % 10000 / 100, g_iTodayOfTrading % 100);
                AUTD_QueryCurrentForm->StatusBarQuery->Panels->Items[4]->Text = dateTrade.FormatString("yyyy-mm-dd");
            }
        }
    }


    g_valInitQueryData = val;
    
}
//---------------------------------------------------------------------------
void __fastcall QueryLoginInitReq()
{
    //���
    {
        std::vector<pubTranCode::Storage*>::iterator it;
        for(it = g_vecStorageList.begin(); it != g_vecStorageList.end(); ++it)
        {
            delete (*it);
        }
        g_vecStorageList.clear();
    }

    //�ֲ�
    {
        std::vector<pubTranCode::Position*>::iterator it;
        for(it = g_vecPositionList.begin(); it != g_vecPositionList.end(); ++it)
        {
            delete (*it);
        }
        g_vecPositionList.clear();
    }

    //���׷���
    {
        std::vector<pubTranCode::TradeFeeRate*>::iterator it;
        for(it = g_vecTradeFeeRateList.begin(); it != g_vecTradeFeeRateList.end(); ++it)
        {
            delete *it;
        }
        g_vecTradeFeeRateList.clear();
    }


    Json::Value val;
    AUTD_HttpDealForm->ToBackHttp(hcLoginInitialization, val);
}
//---------------------------------------------------------------------------
void __fastcall QueryLoginInitFail()
{

}
//---------------------------------------------------------------------------
//�û�������־��¼
void __fastcall OperationRecordReq(const AnsiString &msg)
{
    Json::Value valRec;
  
    //valRec["date"] = TDateTime::CurrentDateTime().FormatString("yyyymmdd").ToInt();
    //valRec["time"] = TDateTime::CurrentDateTime().FormatString("hhnnss").ToInt();
    valRec["operationMsg"] = msg.c_str();
    AUTD_HttpDealForm->ToBackHttp(hcOperationRecord, valRec);
}
//---------------------------------------------------------------------------

//Ϊ���㸡��ӯ���ڽ��׸�ʱ��ȡ��
double __fastcall GetPriceForProfitloss(TPriceItem *priceItem)
{
    //������״̬��ȡ���۽��й�ֵ���ڳ�ʼ����ɡ����̡����Ͼ���״̬��ȡ�������й�ֵ���ھ��۴�ϡ��������ס����ס���ͣ״̬��ȡ���¼۽��й�ֵ

    EscapeStruct escape;
    //�г�״̬
    escape.Type = 1;
    escape.Code = priceItem->marketState;
    GetEscape("MarketstateContext", escape);
    int iMarketState = escape.Index;

    //��ǰ����״̬
    escape.Type = 1;
    escape.Code = priceItem->tradeState;
    GetEscape("TradestateContext", escape);
    //��¼��ǰ����״̬
    int iTradestate = escape.Index;

    double price2Cal = priceItem->last;
    if(iMarketState == mstatClose)
    {
        price2Cal = priceItem->average;//����
    }
    else if(iMarketState == mstatInit // ��ʼ�����
        || iMarketState == mstatOpen //����
        || iTradestate == tsGRPOrder ) // ���۱���
    {
        price2Cal = priceItem->lastSettle; //�����
    }
    else if(iTradestate == tsGRPMatch //���۴��
        || iTradestate == tsNormal //��������
        || iMarketState == mstatTrade //����
        || iMarketState == mstatPause)//��ͣ
    {
        price2Cal = priceItem->last;
    }

    if(price2Cal == 0)
    {
        price2Cal = priceItem->lastSettle; //�����
    }

    return price2Cal;
}

//�б��п�����Ӧ
void __fastcall StringGridsSelfAdjust(TExtStringGrid *sg, int *&ColWidthBuff)
{

//    int loc =(int) sg;
//    loc = loc + sizeof(TCustomControl) + sizeof(TGridCoord) + sizeof(Forms::TFormBorderStyle) + sizeof(bool) + sizeof(int) + 4 - 2;
//    int *privateIntPtr = (*((int**)loc));

    int numOfCol = sg->ColCount;
    int gridWidth = sg->ClientWidth;
    if(gridWidth <= 0)
    {
        return;
    }
    int gridlineWidth = sg->GridLineWidth;
    double ColTotalWidth = gridlineWidth;
    int ColTotalWidthFixed = 0;
    int fixedLast = sg->FixedRows - 1;
    //���蹴ѡ��ʼ�����ڵ�һ��
    int fixedCol = 0;
    if(sg->ShowCheckBox)
    {
        fixedCol = 1;//sg->FixedCols;
        sg->ColWidths[0] = 33;
    }

    for(int i = 0; i < fixedCol; ++i)
    {
        ColWidthBuff[i] = sg->ColWidths[i];
        ColTotalWidthFixed += ColWidthBuff[i] + gridlineWidth;
    }
    for(int i = fixedCol; i < numOfCol; ++i)
    {
        if(sg->ColWidths[i] > 0)
        {
            int minWidth = 0;
            int textCnt = 0;
            int pos = 0;
            for(int j = fixedLast; j < sg->RowCount; ++j)
            {
                AnsiString txt = sg->Cells[i][j];
                int width = sg->Canvas->TextWidth(sg->Cells[i][j]);
                if(width > minWidth)
                {
                    minWidth = width;
                    pos = j;
                }
            }
            textCnt = sg->Cells[i][pos].Length();
            ColWidthBuff[i] = minWidth + 2 * textCnt;
            ColTotalWidth += ColWidthBuff[i] + gridlineWidth;
        }
    }
    double ratio = ColTotalWidth / (gridWidth - ColTotalWidthFixed);
    int inhanceTotalWidth = 0;
    if(ratio < 1.0)
    {
        for(int i = fixedCol; i < numOfCol; ++i)
        {
            if(sg->ColWidths[i] > 0)
            {
                double dTemp = ((double)ColWidthBuff[i]) / ratio;
                int iTemp = ((double)ColWidthBuff[i]) / ratio;

                if(dTemp - iTemp > 0.5)
                {
                    ++iTemp;
                }
                //sg->ColWidths[i] = iTemp;
                ColWidthBuff[i] = iTemp;
                inhanceTotalWidth += iTemp + gridlineWidth;
            }
        }
        int deviation = inhanceTotalWidth - (gridWidth - ColTotalWidthFixed);
        if(deviation >= 0)
        {
            //sg->ColWidths[numOfCol - 1] = sg->ColWidths[numOfCol - 1] - (preciseTotalWidth - (gridWidth - ColTotalWidthFixed - GRID_WIDTH_ADJUST));
            int cnt = fixedCol;
            for(int j = 0; j < deviation;)
            {
                if(sg->ColWidths[cnt] > 0)
                {
                    ColWidthBuff[cnt] = ColWidthBuff[cnt] - 1;
                    ++j;
                }
                ++cnt;
                if(cnt >= numOfCol)
                {
                    cnt = fixedCol;
                }
            }
        }
        else
        {
            int cnt = fixedCol;
            for(int j = 0; j < (-deviation);)
            {
                if(sg->ColWidths[cnt] > 0)
                {
                    ColWidthBuff[cnt] = ColWidthBuff[cnt] + 1;
                    ++j;
                }
                ++cnt;
                if(cnt >= numOfCol)
                {
                    cnt = fixedCol;
                }
            }
        }
        //ʵ���趨���
        for(int i = fixedCol; i < numOfCol; ++i)
        {
            if(sg->ColWidths[i] > 0)
            {
                sg->ColWidths[i] = ColWidthBuff[i];
               // privateIntPtr[i + 1] = ColWidthBuff[i];
            }
        }
//        void * ptr = sg;
//        int loc =(int) ptr;
//        loc = loc + sizeof(TGridCoord) + sizeof(Forms::TFormBorderStyle) + sizeof(bool) + sizeof(int);
//        ptr = (void*)loc;
//
//        int *temp;
//        temp =  ColWidthBuff;
//        ColWidthBuff = (int*)ptr;
//        ptr = temp;
//        //sg->ColWidths[0] =  sg->ColWidths[0] + 1;
    }
    else
    {
        for(int i = 0; i < numOfCol; ++i)
        {
            if(sg->ColWidths[i] > 0)
            {
                sg->ColWidths[i] = ColWidthBuff[i];
                //privateIntPtr[i] = ColWidthBuff[i];
            }
        }
//        void * ptr = sg;
//                int loc =(int) ptr;
//                loc = loc + sizeof(TGridCoord) + sizeof(Forms::TFormBorderStyle) + sizeof(bool) + sizeof(int);
//                ptr = (void*)loc;
//
//                int *temp;
//                temp =  ColWidthBuff;
//                ColWidthBuff = (int*)ptr;
//                ptr = temp;
    }
}
//����С�����ȷλ������
bool __fastcall ControlExtEditInputPrecision(TObject *Sender)
{
    TExtEdit *edit = (TExtEdit*)Sender;
    AnsiString text =  edit->Text;
    int pos = text.Pos(".");
    if(pos > 0)
    {
        int precisionNow = text.Length() - pos;
        int precision = edit->Precsion;
        if(precisionNow > precision)
        {
            edit->Text = text.SubString(1, text.Length() - (precisionNow - precision));
            Application->MessageBox(AnsiString(AnsiString("����С����������λ������") + precision + "λ��Ч��ֵ��").c_str(), "��ʾ", MB_OK | MB_ICONASTERISK);
            //���ý���
            SetControlFocus(edit);
            return true;
        }
    }
    return false;
}
//���°�ť���
void __fastcall SetPriceUpAndDown(TObject *Sender, bool up)
{
    TExtEdit *edtPrice =  (TExtEdit*)Sender;
    try
    {
        if(edtPrice->Text.Trim() == "")
        {
            edtPrice->Text = "0";
        }
        if(edtPrice->DataType == dtInteger)
        {
            int iPrice = edtPrice->Value.ToInt();
            if(up)
            {
                ++iPrice;
            }
            else
            {
                --iPrice;
            }
            //����С��0
            if(iPrice < 0)
            {
                iPrice = 0;
            }
            edtPrice->Text = iPrice;
        }
        else
        {
#ifdef THOUSANDSEP
            double dPrice = StringReplace(edtPrice->Value, ",", "", TReplaceFlags() << rfReplaceAll).ToDouble();
#else
            double dPrice = edtPrice->Value.ToDouble();
#endif

            int precsion = edtPrice->Precsion;
            if(up)
            {
                dPrice = dPrice + pow(10, precsion * -1);
            }
            else
            {
                dPrice = dPrice - pow(10, precsion * -1);
            }
            //����С��0
            if(dPrice < 0)
            {
                dPrice = 0;
            }
            edtPrice->Text = StringReplace(FloatToStrF(dPrice, ffNumber, 18, edtPrice->Precsion), ",", "", TReplaceFlags() << rfReplaceAll);
        }
    }
    catch(EConvertError &e)
    {

    }
}
/*
    �жϽ���Ƿ�Ϊ����
*/
bool __fastcall IsNagetiveFloat(const AnsiString str)
{
    if( str.Trim().IsEmpty())
        return false;
        
    AnsiString str1 = str;
    float tmpamt ;
    tmpamt = StrToFloat(str1);
    
    if(tmpamt < 0.00 )
        return true;
    else
        return false;
}

/*
    �ַ������滻,�滻�ַ����еĶ���
*/
AnsiString __fastcall StringReplaceNum(AnsiString number)
{
    AnsiString strTmp = StringReplace(number, ",", "", TReplaceFlags() << rfReplaceAll);

    return strTmp;
}

//�Ƿ���ȷ��key
bool __fastcall IsConfirmKey(WORD &Key)
{
    if(Key == 32  //�ո�
        || Key == 13) //�س�
    {
        return true;
    }
    else
    {
        return false;
    }
}

void __fastcall QueryETFQuoRecv(Json::Value &val)
{
    AnsiString strTemp = "";
    int count = 0;
    //count = AnsiString(val["count"].asCString()).ToIntDef(0);
    AnsiString listName = AnsiString("send") + HttpContext[hcQueryETFQuo].PackName;
    count = val[listName.c_str()]["etfList"].size();

    std::map<std::string, pubTranCode::ETFQuotation*>::iterator it;
    for(it = g_mapETFQuoList.begin(); it != g_mapETFQuoList.end(); ++it)
    {
        delete (it->second);
    }
    g_mapETFQuoList.clear();
    
    for(int i = 0; i < count; ++i)
    {
        pubTranCode::ETFQuotation *etfQuo = new pubTranCode::ETFQuotation;
        etfQuo->InitByJson(val[listName.c_str()], i);

        g_mapETFQuoList.insert(pair<std::string, pubTranCode::ETFQuotation*>(std::string(etfQuo->fundCode), etfQuo));
    }
}

void __fastcall QueryETFQuoReq()
{
    Json::Value valQuery;

    valQuery["tradeDate"] = g_iTodayOfTrading;

    AUTD_HttpDealForm->ToBackHttp(hcQueryETFQuo, valQuery);
}

void __fastcall QueryETFQuoFail()
{

}

void __fastcall QueryETFFundCodeRecv(Json::Value &val)
{
    AnsiString strTemp = "";
    int count = 0;
    //count = AnsiString(val["count"].asCString()).ToIntDef(0);
    AnsiString listName = AnsiString("send") + HttpContext[hcQueryFundCode].PackName;
    count = val[listName.c_str()]["fundInfoList"].size();

    std::vector<pubTranCode::TETFFundCode*>::iterator it;
    for(it=g_vecETFFundCodeList.begin(); it!=g_vecETFFundCodeList.end(); ++it)
    {
        delete (*it);
    }
    g_vecETFFundCodeList.clear();

    for(int i = 0; i < count; ++i)
    {
        pubTranCode::TETFFundCode *etfFundCode = new pubTranCode::TETFFundCode;
        Json::Value valFundCode = val[listName.c_str()]["fundInfoList"][i];

        etfFundCode->InitByJson(valFundCode);

        g_vecETFFundCodeList.push_back(etfFundCode);
    }
}

//�ҵ�ϵͳ��ѯ
void __fastcall QueryAllOrdsOrder()
{
    Json::Value valQuery;

    valQuery["tradeDate"] = g_iTodayOfTrading;

    AUTD_HttpDealForm->ToBackHttp(hcQueryETFQuo, valQuery);    
}
#pragma package(smart_init)
