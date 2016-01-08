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

/**最大可交易手数试算*/
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

//交易失败(更新请求)list
Json::Value g_valInfoUpdateList;


//交易类型
const AnsiString TradeTypeContext = "现货交易,0;延期交易,2;交收申报,4;中立仓,6";

//交易类型 (改价下单专用子集,只用于设置combobox)
const AnsiString TradeTypeReOrderContext = "现货交易,0;延期交易,2";

//市场类型
const AnsiString MarkettypeContext = "现货,s;延期,d;期货,f;远期,w";

//可交割状态
const AnsiString DeliveryStateContext = "不可交割中立仓申报,0;可交割中立仓申报,1";

//买卖方向
const AnsiString BuyOrSellContext = "买入,b;卖出,s";

//买卖方向&交收方向 (查询用特殊处理)
const AnsiString BSJSContext = "买入,b;卖出,s;收货,i;交货,o";

//交收方向
const AnsiString JiaoOrShouContext = "收货,b;交货,s";

//委托状态
const AnsiString OrderStateContext = "未成交,o;部分成交,p;完全成交,c;用户删除,d;系统删除,s;待申报,i;已报送,m;报送失败,n;正在发起,h";

//开平仓标志
const AnsiString OffsetFlagContext = "开仓,0;平仓,1;强制平仓,2";
const AnsiString OffsetFlagContextR = "开仓,0;平仓,1";

//支付方向
const AnsiString PayDirectionContext = "未明,0;多支付空,1;空支付多,2;未定,3";

//申报状态(提货申请)
const AnsiString ApplyStateContext = "成功,1;提货单不存在,2;提货单已提货,3;提货单已撤单,4;失败,5;未明,6";

//提货单状态
const AnsiString PickupApplyStateContext = "申请提货,1;会员当日撤单,3;会员次日撤单,4;系统撤单,5;金交所已受理,6;银行已受理,7;\
    提货计划已安排,8;交提出库,9;未明,10;结算完成,11;审批通过,12;审批不通过,13;审批失败,14;可打印提货单,15";

//合约交易状态
const AnsiString TradestateContext = "初始化中,0;初始化完成,1;开盘,2;竞价报单,3;竞价撮合,4;连续交易,5;暂停,6;\
    交割申报,7;交割申报结束,8;中立仓申报,9;交割申报撮合,A;收盘,B";

//转账
const AnsiString InOutAmtTypeContext = "转入,0;转出,1";

//出入金状态
const AnsiString InOutAmtStateContext = "成功,0;失败,1;未明未处理,2;未明手工已处理,3;未发起,4";

//资金流水状态
const AnsiString FundsSeqStateContext = "成功,0;失败,1;未明未处理,2;未明手工已处理,3;未发起,4";

//是否是中立仓
const AnsiString IsMidContext = "否,0;是,1";

//交易类型(资金流水)
const AnsiString FundsSeqType = "转入,0;转出,1;冻结,2;解冻,3;增加,4;扣除,5;收取,X";

//费用类型
const AnsiString FundsTypeContext = "保证金,00;交易资金,01;买入货权仓储费,02;超期补偿费,03;递延费,04;运保费,05;保险费,06;手续费,07;\
    条块升贴水,08;交割手续费,09;交易所交割违约金,10;会员交割违约金,11;溢短费,12;品种升贴水,13;质押登记费,14;租借登记费,15;\
    会员平仓违约,16;提货出库费,17;平仓损益,18;提货冻结资金,19;利息,20;利息税,21;结算盈亏,22;开户费,23;银行开户手续费,24;\
    银行提货手续费,25;出库仓储费,26;税后利息,27;清算资金,28;清算银行交易费手续费,29;清算银行提货手续费,30;\
    ETF资金,31;过户费,32;现金差额,33";

//库存变化类型
const AnsiString TransTypeContext = "入库,1;提货申请,2;提货申请修改,3;当日撤销提货申请,4;自提出库,5;交提出库,6;质押（质权方）,7;\
    质押（出质方）,8;质押注销（质权方）,9;质押注销（出质方）,A;租借（借出）,B;租借（借入）,C;法律冻结,D;法律冻结解冻,E;\
    现货买入,F;现货卖出,G;处置质物,H;出库,K;大宗交易买入,M;大宗交易卖出,N;划出库存,O;划入库存,P;充抵做市商透支,Q;减做市商入库库存,R;\
    非即期卖出,S;非即期买入,T;次日撤销提货申请,U;系统撤销提货申请,V;\
    认购（转出）对客户,11;认购（转入）对基金公司,12;认购失败（转出）对基金公司,13;认购失败（转入）对客户,14;申购（转出）对客户,15;\
    申购（转入）对基金公司,16;赎回（转出）对基金公司,17;赎回（转入）对客户,18;应急黄金ETF非交易过户转出,19;应急黄金ETF非交易过户转入,20;非交易过户转出,25;非交易过户转入,26";

//通知单类型
const AnsiString RequizitionTypeContext = "追保通知单,0;强平通知单,1";

//风险等级
const AnsiString RiskGradeContext = "红色强平级,0;橙色追保级,1;绿色安全级,2;红色追保级,3";

//市场状态
const AnsiString MarketstateContext = "初始化中,0;初始化完成,1;开市,2;交易,3;暂停,4;收市,5";

//改价下单标识
const AnsiString ReOrderChangeFlagContext = "已发起,0;已处理,1";

//认申赎查询
const AnsiString ETFTradeType = "认购,020;申购,022;赎回,024";
                                             
//确认标志
const AnsiString ETFStatus = "认购请求已受理,0;认购请求已受理,1;认购请求已受理,204;认购失败,205;认购成功,206;认购失败,207;\
                            申购请求已受理,3;申购请求已受理,4;申购成功,104;申购失败,105;\
                            赎回请求已受理,6;赎回请求已受理,7;赎回失败,8;赎回失败,9;赎回成功,304";

//认申赎确认标志查询条件
const AnsiString ETFQueryCond = "认购请求已受理,0;认购失败,1;认购成功,2;申购请求已受理,3;申购成功,4;申购失败,5;赎回请求已受理,6;赎回失败,7;赎回成功,8";

//基金状态
const AnsiString FundState = "不可交易,0;可交易,1";

//客户绑定状态                  "未绑定,0;本地绑定待确认,100;本地解绑待确认,101;绑定待确认,701;绑定已完成,702;绑定已失败,703;解绑定待确认,801;解绑定已完成,802;解绑定已失败,803";
const AnsiString FundStatus = "未绑定,0;绑定请求已受理,100;解绑定请求已受理,101;绑定请求已受理,701;已绑定,702;未绑定,703;解绑定请求已受理,801;未绑定,802;已绑定,803";

//当日是否允许申购/赎回标志
const AnsiString ETFSgShFlag = "不允许申购/赎回,0;申购和赎回皆允许,1;仅允许申购,2;仅允许赎回,3";

const char *VarietyTypeStr[] = {"1", "2", "3"};

const AnsiString Str_Ordsys_Type = "止损,l;止盈,s;条件单,c;移动止损,ml;预委托,p";
const AnsiString Str_Ordsys_Status = "当前有效,o;已经发起,c;用户撤销,d;系统撤销,s;发起失败,f;正在发起,h";
const AnsiString Str_Ordsys_Expire = "当日有效,d;永久有效,f";
const AnsiString Str_Ordsys_Condition = "大于等于,>=;小于等于,<=;等于,=;大于,>;小于,<";
const AnsiString Str_Ordsys_OrdPriceType = "自定义价,c;最新价,l;卖一价,s;买一价,b";

const AnsiString Str_LimitOptionCont = "不启用,0;启用,1";

const AnsiString Str_OrderTypeContext = "普通委托,0;预委托,1";

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
    if (grid->RowCount <= 2) //为初始页
    {
        if (grid->Cells[0][1].IsEmpty())
            row = 1;
        else
            row = 2;
    }
    else if ((grid->RowCount>2) && (!grid->Cells[0][1].IsEmpty()))
    {
       row = grid->RowCount; //下一页

       //如果是最后一页
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
                if (tmpvallst->Count > 3)  //说明有额外配置信息
                {
                    char datatype[2];
                    char dataFormat[25];
                    memset(datatype, 0, 2);
                    memset(dataFormat, 0, 25);
                    sscanf(tmpvallst->Strings[3].c_str(), "%[^,],%[^,]", datatype, dataFormat);

                    if (datatype[0] == 'e') //需要转义的代码
                    {
                        AnsiString format = AnsiString(dataFormat);
                        EscapeStruct escape;
                        escape.Type = 1;
                        escape.Code = (*pFields)[skeyName].asCString();
                        GetEscape(format.c_str(), escape);
                        grid->Cells[k][row] = escape.Text;
                    }
                    else if (datatype[0] == 't')  //为日期形式
                    {
                        int format = AnsiString(dataFormat).ToInt();
                        if (format == 8) //date
                            grid->Cells[k][row] = Date2Str(AnsiString((*pFields)[skeyName].asCString()).ToIntDef(0), '-').c_str();
                        else if (format == 6) //time
                            grid->Cells[k][row] = Time2Str(AnsiString((*pFields)[skeyName].asCString()).ToIntDef(0), ':').c_str();
                        else
                            grid->Cells[k][row] = AnsiString((*pFields)[skeyName].asCString()).ToIntDef(0);
                    }
                    else if (datatype[0] == 'f') //浮点数数据
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
                    else if(datatype[0] == 'i') //整形数据
                    {
                        grid->Cells[k][row] = AnsiString((*pFields)[skeyName].asCString()) + " ";
                    }
                    else if(datatype[0] == '%')//百分比数据
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
                if (tmpvallst->Count > 3) //说明有额外配置信息
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
    if (grid->RowCount <= 2) //为初始页
    {
        if (grid->Cells[0][1].IsEmpty())
            row = 1;
        else
            row = 2;
    }
    else if ((grid->RowCount>2) && (!grid->Cells[0][1].IsEmpty()))
    {
       row = grid->RowCount; //下一页

       //如果是最后一页
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
                if (tmpvallst->Count > 3)  //说明有额外配置信息
                {
                    char datatype[2];
                    char dataFormat[25];
                    memset(datatype, 0, 2);
                    memset(dataFormat, 0, 25);
                    sscanf(tmpvallst->Strings[3].c_str(), "%[^,],%[^,]", datatype, dataFormat);

                    if (datatype[0] == 'e') //需要转义的代码
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
            else if ((*pFields)[skeyName].isInt())  //下拉框值 和 日期值
            {
                if (tmpvallst->Count > 3)  //说明有额外配置信息
                {
                    char datatype[2];
                    char dataFormat[25];
                    memset(datatype, 0, 2);
                    memset(dataFormat, 0, 25);
                    sscanf(tmpvallst->Strings[3].c_str(), "%[^,],%[^,]", datatype, dataFormat);

                    if (datatype[0] == 'e') //需要转义的代码
                    {
                        AnsiString format = AnsiString(dataFormat);
                        EscapeStruct escape;
                        escape.Type = 1;
                        escape.Code = (*pFields)[skeyName].asInt();
                        GetEscape(format.c_str(), escape);
                        grid->Cells[k][row] = escape.Text;
                    }
                    else if (datatype[0] == 't')  //为日期形式
                    {
                        int format = AnsiString(dataFormat).ToInt();
                        if (format == 8) //date
                            grid->Cells[k][row] = Date2Str((*pFields)[skeyName].asInt(), '-').c_str();
                        else if (format == 6) //time
                            grid->Cells[k][row] = Time2Str((*pFields)[skeyName].asInt(), ':').c_str();
                        else
                            grid->Cells[k][row] = (*pFields)[skeyName].asInt();
                    }
                    else if(datatype[0] == 'i') //整形数据
                    {
                        grid->Cells[k][row] = AnsiString((*pFields)[skeyName].asInt()) + " ";
                    }
                    else if (datatype[0] == 'f') //为0的浮点数数据
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
            else if ((*pFields)[skeyName].isDouble())  //双精度型
            {
                if (tmpvallst->Count > 3)  //说明有额外配置信息
                {
                    char datatype[2];
                    char dataFormat[25];
                    memset(datatype, 0, 2);
                    memset(dataFormat, 0, 25);
                    sscanf(tmpvallst->Strings[3].c_str(), "%[^,],%[^,]", datatype, dataFormat);

                    if (datatype[0] == 'e') //需要转义的代码
                    {
                        AnsiString format = AnsiString(dataFormat);
                        EscapeStruct escape;
                        escape.Type = 1;
                        escape.Code = (*pFields)[skeyName].asDouble();
                        GetEscape(format.c_str(), escape);
                        grid->Cells[k][row] = escape.Text;
                    }
                    else if (datatype[0] == 'f') //浮点数数据
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
                    else if(datatype[0] == '%')//百分比数据
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
    //交易类型
    if(context == "TradeTypeContext")
    {
        return TradeTypeContext;
    }
    //交易类型 (改价下单专用子集,只用于设置combobox)
    else if(context == "TradeTypeReOrderContext")
    {
        return TradeTypeReOrderContext;
    }
    //市场类型
    else if(context == "MarkettypeContext")
    {
        return MarkettypeContext;
    }
    //可交割状态
    else if(context == "DeliveryStateContext")
    {
        return DeliveryStateContext;
    }
    //买卖方向
    else if(context == "BuyOrSellContext")
    {
        return BuyOrSellContext;
    }
    //买卖方向&交收方向 (查询用特殊处理)
    else if(context == "BSJSContext")
    {
        return BSJSContext;
    }
    //交收方向
    else if(context == "JiaoOrShouContext")
    {
        return JiaoOrShouContext;
    }
    //委托状态
    else if(context == "OrderStateContext")
    {
        return OrderStateContext;
    }
    //开平仓标志
    else if(context == "OffsetFlagContext")
    {
        return OffsetFlagContext;
    }
    //开平仓标志 (界面上显示)
    else if(context == "OffsetFlagContextR")
    {
        return OffsetFlagContextR;
    }
    //支付方向
    else if(context == "PayDirectionContext")
    {
        return PayDirectionContext;
    }
    //申报状态(提货申请)
    else if(context == "ApplyStateContext")
    {
        return ApplyStateContext;
    }
    //提货单状态
    else if(context == "PickupApplyStateContext")
    {
        return PickupApplyStateContext;
    }
    //合约交易状态
    else if(context == "TradestateContext")
    {
        return TradestateContext;
    }
    //转账
    else if(context == "InOutAmtTypeContext")
    {
        return InOutAmtTypeContext;
    }
    //出入金状态
    else if(context == "InOutAmtStateContext")
    {
        return InOutAmtStateContext;
    }
    else if(context == "FundsSeqStateContext")
    {
        return FundsSeqStateContext;
    }
    //是否是中立仓
    else if(context == "IsMidContext")
    {
        return IsMidContext;
    }
    //交易类型(资金流水)
    else if(context == "FundsSeqType")
    {
        return FundsSeqType;
    }
    //费用类型
    else if(context == "FundsTypeContext")
    {
        return FundsTypeContext;
    }
    //库存变化类型
    else if(context == "TransTypeContext")
    {
        return TransTypeContext;
    }
    //通知单类型
    else if(context == "RequizitionTypeContext")
    {
        return RequizitionTypeContext;
    }
    //风险等级
    else if(context == "RiskGradeContext")
    {
        return RiskGradeContext;
    }
    //市场状态
    else if(context == "MarketstateContext")
    {
        return MarketstateContext;
    }
    //改价下单标识
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

        //根据索引取文字和代码
        if(escape.Type == 0)
        {
            memset(code, 0, 30);
            memset(text, 0, 100);
            sscanf(slist->Strings[escape.Index].c_str(),"%[^,],%[^,]", &text, &code);
            escape.Code = code;
            escape.Text = text;
        }
        //根据代号取文字和索引
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
        //根据文字取代号和索引
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

    //金交所交易日
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

/**最大可交易手数试算*/
//资金
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
    ////重置事件
    //ResetEvent(m_eventQueryCustFunds);
    AUTD_HttpDealForm->ToBackHttp(hcQueryCustFundsInit, valQuery);
}
//---------------------------------------------------------------------------
//库存
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
    ////重置事件
    //ResetEvent(m_eventQueryStorage);
    AUTD_HttpDealForm->ToBackHttp(hcQueryStorageInit, valQuery);
}
//---------------------------------------------------------------------------
//持仓
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

    //页码
    valQuery["pageNo"] = 1;
    ////重置事件
    //ResetEvent(m_eventQueryPosition);
    AUTD_HttpDealForm->ToBackHttp(hcQueryPositionInit, valQuery);
}
//---------------------------------------------------------------------------
//交易费率
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
//交易费率获取2
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
    double orderSpread = 0;     //触发价与委托价差
    double Threshold = 0;       //产品阈值
    char cFreezeSort[21];       //交割品种冻结顺序
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
    //交易费率查询
    Json::Value valQuery;
    std::vector<pubTranCode::TradeFeeRate*>::iterator it;
    for(it = g_vecTradeFeeRateList.begin(); it != g_vecTradeFeeRateList.end(); ++it)
    {
        delete *it;
    }
    g_vecTradeFeeRateList.clear();

    //页码
    valQuery["pageNo"] = 1;
    ////重置事件
    //ResetEvent(m_eventQueryTradeFeeRate);
    AUTD_HttpDealForm->ToBackHttp(hcQueryTradeFeeRateInit, valQuery);
}
//---------------------------------------------------------------------------
//系统参数获取
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
                g_iInfoUpdateTime = AnsiString(sysParam->value).ToDouble();        //信息更新请求间隔，单位:秒
            }
            else if(strcmp(sysParam->name, "ZSZYLimit") == 0)  //客户端止损止盈最大设置笔数上限
            {
                g_iZSZYLimit = AnsiString(sysParam->value).ToInt();
            }
            else if(strcmp(sysParam->name, "TJDLimit") == 0)  //客户端条件单最大设置笔数上限
            {
                g_iTJDLimit = AnsiString(sysParam->value).ToInt();
            }
            else if(strcmp(sysParam->name, "TJDPriceLimit") == 0)  //客户端条件单连续笔数上限
            {
                g_iTJDPriceLimit = AnsiString(sysParam->value).ToInt();
            }
            else if (strcmp(sysParam->name, "IsOpenETF") == 0)  //ETF功能是否开启
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
    if(AnsiString(val[listName.c_str()]["endFlag"].asCString()).ToIntDef(0) == 1) //还有后续包
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

    //保证信息更新请求间隔 > 0
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
                    g_iInfoUpdateTime = AnsiString(param.value).ToDouble();        //信息更新请求间隔，单位:秒
                }
                else if(strcmp(param.name, "ZSZYLimit") == 0)  //客户端止损止盈最大设置笔数上限
                {
                    g_iZSZYLimit = AnsiString(param.value).ToInt();
                }
                else if(strcmp(param.name, "TJDLimit") == 0)  //客户端条件单最大设置笔数上限
                {
                    g_iTJDLimit = AnsiString(param.value).ToInt();
                }
                else if(strcmp(param.name, "TJDPriceLimit") == 0)  //客户端条件单连续笔数上限
                {
                    g_iTJDPriceLimit = AnsiString(param.value).ToInt();
                }
                else if (strcmp(param.name, "IsOpenETF") == 0)  //ETF功能是否开启
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
                else if (strcmp(param.name, "UpdateETFList") == 0)  //ETF申赎清单变更通知
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
                AnsiString strMsg = "更新系统参数错误,参数信息：" + asTemp;
                PackagePublic->ShowWarning(strMsg);
            }
        }
	}
	delete tmpList1;
    tmpList1 = NULL;
    
    //保证信息更新请求间隔 > 0
    if(g_iInfoUpdateTime <= 0)
    {
        g_iInfoUpdateTime = 10;
    }
    //重新设置定时器
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

    return AnsiString("出入金转账成功") + tranType + "资金：\n" + FloatToStrF(amount, ffNumber, 18, 2);

}
//---------------------------------------------------------------------------
AnsiString __fastcall DoPickupApplyReturn(Json::Value &val)
{
    AnsiString packName = AnsiString("send") + HttpContext[hcDoPickupApply].PackName;
    AnsiString localPickupNo = val[packName.c_str()]["localPickupNo"].asCString();
    return AnsiString("提货申请成功！\n") + "提货单编号：" + localPickupNo;
}
//---------------------------------------------------------------------------
//客户上日风险查询
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
    if(AnsiString(val[listName.c_str()]["endFlag"].asCString()).ToIntDef(0) == 1) //还有后续包
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
        //查询起始日期
        valQueryYesterdayRisk["startDate"] = date.FormatString("yyyymmdd").ToInt();
        //查询结束日期
        valQueryYesterdayRisk["endDate"] = date.FormatString("yyyymmdd").ToInt();
        */
        //查询起始日期
        valQueryYesterdayRisk["startDate"] = "20120201";
        //查询结束日期
        valQueryYesterdayRisk["endDate"] = "20120228";

    }
    //页码
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
        // 获取金交所交易日
        GetExtradeDateReq();
        if(INVALID_EXTRADEDATE == g_iTodayOfTrading)
            return;
        
        TDateTime date = EncodeDate(g_iTodayOfTrading / 10000, g_iTodayOfTrading % 10000 / 100, g_iTodayOfTrading % 100);
        date = date - 1;
        */
        valQueryYesterdayRisk.clear();
        /*
        //查询起始日期
        valQueryYesterdayRisk["startDate"] = date.FormatString("yyyymmdd").ToInt();
        //查询结束日期
        valQueryYesterdayRisk["endDate"] = date.FormatString("yyyymmdd").ToInt();
        */
        //查询起始日期
        valQueryYesterdayRisk["startDate"] = "20120201";
        //查询结束日期
        valQueryYesterdayRisk["endDate"] = "20120228";

    }
    //页码
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
    //查询系统信息
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
//登陆时一次性初始化数据查询
void __fastcall QueryLoginInitRecv(Json::Value &val)
{
    AnsiString listName = AnsiString("send") + HttpContext[hcLoginInitialization].PackName;
    //资金
    {
        g_Funds.InitByJson(val[listName.c_str()]);
        g_bQueryCustFundsSucc = true;
        //SetEvent(m_eventQueryCustFunds);
        //g_valCustFundsInit = val;
    }

    //库存
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

    //持仓
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

    //交易费率
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

    //系统参数
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
                    g_iInfoUpdateTime = AnsiString(sysParam->value).ToDouble();        //信息更新请求间隔，单位:秒
                }
                else if(strcmp(sysParam->name, "ZSZYLimit") == 0)  //客户端止损止盈最大设置笔数上限
                {
                    g_iZSZYLimit = AnsiString(sysParam->value).ToInt();
                }
                else if(strcmp(sysParam->name, "TJDLimit") == 0)  //客户端条件单最大设置笔数上限
                {
                    g_iTJDLimit = AnsiString(sysParam->value).ToInt();
                }
                else if(strcmp(sysParam->name, "TJDPriceLimit") == 0)  //客户端条件单连续笔数上限
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
                else if (strcmp(sysParam->name, "IsOpenETF") == 0)  //ETF功能是否开启
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
        if(AnsiString(val[listName.c_str()]["endFlag"].asCString()).ToIntDef(0) == 1) //?1óDoóD?°ü
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
        //保证信息更新请求间隔 > 0
        if(g_iInfoUpdateTime <= 0)
        {
            g_iInfoUpdateTime = 10;
        }
    }

    //客户上日风险
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
        if(AnsiString(val[listName.c_str()]["endFlag"].asCString()).ToIntDef(0) == 1) //?1óDoóD?°ü
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

    //金交所交易日
    {
#ifndef PERFORMANCETEST
        AnsiString strTemp = val[listName.c_str()]["exTradeDate"].asCString();
#else
        AnsiString strTemp = val[listName.c_str()]["exTradeDate"].asInt();
#endif

        g_iTodayOfTrading = strTemp.ToIntDef(INVALID_EXTRADEDATE);

        //金交所交易日
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
    //库存
    {
        std::vector<pubTranCode::Storage*>::iterator it;
        for(it = g_vecStorageList.begin(); it != g_vecStorageList.end(); ++it)
        {
            delete (*it);
        }
        g_vecStorageList.clear();
    }

    //持仓
    {
        std::vector<pubTranCode::Position*>::iterator it;
        for(it = g_vecPositionList.begin(); it != g_vecPositionList.end(); ++it)
        {
            delete (*it);
        }
        g_vecPositionList.clear();
    }

    //交易费率
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
//用户操作日志记录
void __fastcall OperationRecordReq(const AnsiString &msg)
{
    Json::Value valRec;
  
    //valRec["date"] = TDateTime::CurrentDateTime().FormatString("yyyymmdd").ToInt();
    //valRec["time"] = TDateTime::CurrentDateTime().FormatString("hhnnss").ToInt();
    valRec["operationMsg"] = msg.c_str();
    AUTD_HttpDealForm->ToBackHttp(hcOperationRecord, valRec);
}
//---------------------------------------------------------------------------

//为计算浮动盈亏在交易各时段取价
double __fastcall GetPriceForProfitloss(TPriceItem *priceItem)
{
    //在收市状态下取均价进行估值；在初始化完成、开盘、集合竞价状态下取昨结算进行估值；在竞价撮合、连续交易、交易、暂停状态下取最新价进行估值

    EscapeStruct escape;
    //市场状态
    escape.Type = 1;
    escape.Code = priceItem->marketState;
    GetEscape("MarketstateContext", escape);
    int iMarketState = escape.Index;

    //当前交易状态
    escape.Type = 1;
    escape.Code = priceItem->tradeState;
    GetEscape("TradestateContext", escape);
    //记录当前交易状态
    int iTradestate = escape.Index;

    double price2Cal = priceItem->last;
    if(iMarketState == mstatClose)
    {
        price2Cal = priceItem->average;//均价
    }
    else if(iMarketState == mstatInit // 初始化完成
        || iMarketState == mstatOpen //开市
        || iTradestate == tsGRPOrder ) // 竞价报单
    {
        price2Cal = priceItem->lastSettle; //昨结算
    }
    else if(iTradestate == tsGRPMatch //竞价撮合
        || iTradestate == tsNormal //连续交易
        || iMarketState == mstatTrade //交易
        || iMarketState == mstatPause)//暂停
    {
        price2Cal = priceItem->last;
    }

    if(price2Cal == 0)
    {
        price2Cal = priceItem->lastSettle; //昨结算
    }

    return price2Cal;
}

//列表列宽自适应
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
    //假设勾选框始终是在第一个
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
        //实际设定宽度
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
//控制小数点后精确位数输入
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
            Application->MessageBox(AnsiString(AnsiString("您在小数点后输入的位数超过") + precision + "位有效数值！").c_str(), "提示", MB_OK | MB_ICONASTERISK);
            //设置焦点
            SetControlFocus(edit);
            return true;
        }
    }
    return false;
}
//上下按钮设价
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
            //不得小于0
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
            //不得小于0
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
    判断金额是否为负数
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
    字符串的替换,替换字符串中的逗号
*/
AnsiString __fastcall StringReplaceNum(AnsiString number)
{
    AnsiString strTmp = StringReplace(number, ",", "", TReplaceFlags() << rfReplaceAll);

    return strTmp;
}

//是否是确认key
bool __fastcall IsConfirmKey(WORD &Key)
{
    if(Key == 32  //空格
        || Key == 13) //回车
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

//挂单系统查询
void __fastcall QueryAllOrdsOrder()
{
    Json::Value valQuery;

    valQuery["tradeDate"] = g_iTodayOfTrading;

    AUTD_HttpDealForm->ToBackHttp(hcQueryETFQuo, valQuery);    
}
#pragma package(smart_init)
