#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <fstream>
#include <map>
#include "tools.h"

using namespace std;
using namespace cv;

int str2int(string str);
void help();
void merge(int count,int m,int n,vector<string> paths,string savePath);
void parse(int argc,char** argv,map<string,vector<string>> &args);

int main( int argc, char** argv )
{
    if(argc==1){
        help();
        return 0;
    }

    //解析参数
    map<string,vector<string>> args;
    parse(argc,argv,args);

    //删除旧目录，新建保存目录
    string savePath = args["-s"][0];
//    cout<<savePath<<endl;
#ifdef __linux__
    system(("rm -rf "+savePath).c_str());
    system(("mkdir "+savePath).c_str());
#elif defined(_WIN32)
    system(("rmdir /s /q "+savePath).c_str());
    system(("md "+savePath).c_str());
#else
#error Not soupport this system
#endif

    //提取参数
    auto count = str2int(args["-c"][0]);
    ASSERT(count>0,"合并的图片组数数量小于1");
    auto m = str2int(args["-m"][0]);
    auto n = str2int(args["-n"][0]);
    ASSERT(m>0 && n >0,"m,n必须大于0");
//    printf("count:%d m:%d n:%d\n",count,m,n);

    //提取路径
    vector<string> paths = args["-p"];
//    for(int i=0;i<paths.size();i++){
//        cout<<paths[i]<<" ";
//    }
//    cout<<endl;

    merge(count,m,n,paths,savePath);

    return 0;
}

void help(){
    printf("调用方法: ./MergePicture -c count -m m -n n "
                   "-s save_path -p path1 path2 ... pathm*n\n"
                   "\n"
                   "-c 要合并图片的组数;\n"
                   "-m 合并后横向包含几张图片;\n"
                   "-n 合并后纵向包含几张图片;\n"
                   "-s 指定保存路径;\n"
                   "-p 指定m*n个包含图片名称的txt文件路径;若路径数量\n"
                   "不够m*n，则会用空白补足。-p这个参数必须在前面的参\n"
                   "数之后指定,是最后一个参数\n"
                   "\n"
                   "调用demo:\n"
                   "./MergePicture -c 8 -m 2 -n 2 -s merge -p ./图片组1/src.txt "
                   "./图片组2/src.txt ./图片组3/src.txt ./图片组4/src.txt \n\n\n"
    );
    printf("获取以上提到的txt文件，运行getSrcTxt.sh获得。\n\n");
    printf("生成脚本getSrcTxt.sh中:\n");
    ofstream o("getSrcTxt.sh");
    o<< "#!/usr/bin/env bash\n"
            "\n"
            "if [ $# = 0 ];then\n"
            "   echo '调用方法:./getSrcTxt.sh path type'\n"
            "   echo 'path:图片目录相对路径;type:图片类型比如bmp、jpeg等。'\n"
            "   echo '文件夹中的图片需要用数字命名，才能正确的排序。'\n"
            "else \n"
            "   if [ $# = 2 ];then\n"
            "       if [ ! -d \"$1\" ];then\n"
            "           echo \"不存在目录:$1\"\n"
            "           exit -1\n"
            "       fi\n"
            "\n"
            "       cd \"$1\"\n"
            "\n"
            "       if [ -f \"src.txt\" ];then\n"
            "           rm \"src.txt\"\n"
            "       fi\n"
            "\n"
            "       for a in $(ls *.$2|sort -t. -k1.1n);\n"
            "       do\n"
            "           echo \"$PWD/$a\" >>src.txt\n"
            "       done\n"
            "\n"
            "       cd ..\n"
            "   else\n"
            "       echo '参数不正确。如果路径中间有空格，请用在路径开头和结尾打上英文双引号。'\n"
            "   fi\n"
            "fi"<<endl;
    o.close();
    system("chmod +x getSrcTxt.sh");
    printf("生成完成\n");
}

/**
 * 解析参数
 * @param argc 参数个数
 * @param argv 参数数组
 * @param args 解析结果
 */
void parse(int argc,char** argv,map<string,vector<string>> &args){

    //检查参数数量
    ASSERT(argc>10,"参数数量不足");

    for(int i=1;i<argc;i++){
        if(argv[i][0]=='-'){

            if(strcmp(argv[i],"-c")==0){//count
                ASSERT(args.count("-c")==0,"参数-c重复");
                vector<string> value;
                value.emplace_back(argv[++i]);
                args.insert(make_pair("-c",value));
            }
            else if(strcmp(argv[i],"-m")==0){//m
                ASSERT(args.count("-m")==0,"参数-m重复");
                vector<string> value;
                value.emplace_back(argv[++i]);
                args.insert(make_pair("-m",value));
            }
            else if(strcmp(argv[i],"-n")==0){//n
                ASSERT(args.count("-n")==0,"参数-n重复");
                vector<string> value;
                value.emplace_back(argv[++i]);
                args.insert(make_pair("-n",value));
            }
//            else if(strcmp(argv[i],"-t")==0){//txt name
//                ASSERT(args.count("-t")==0,"参数-t重复");
//                vector<string> value;
//                value.emplace_back(argv[++i]);
//                args.insert(make_pair("-t",value));
//            }
            else if(strcmp(argv[i],"-s")==0){//save path
                ASSERT(args.count("-s")==0,"参数-s重复");
                vector<string> value;
                value.emplace_back(argv[++i]);
                args.insert(make_pair("-s",value));
            }
            else if(strcmp(argv[i],"-p")==0){//各组图片路径
                ASSERT(args.count("-p")==0,"参数-p重复");
                vector<string> value;
                for(int j=++i;j<argc;j++)
                    value.emplace_back(argv[j]);
                args.insert(make_pair("-p",value));
                break;
            }
        }else{
            ASSERT(false,"参数格式错误");
        }
    }

    if(args.size()!=5){
        ASSERT(false,"参数数量不足");
    }
}

/**
 * 字符数组转数字
 * @param str 数字字符数组，例如 char str[]="123";
 * @return 转换成功返回对应数值123,转换失败会抛出异常退出程序
 */
long charArray2Long(char *str){
    char *stop;
    long num = strtol(str,&stop,10);
    if(strlen(stop)>0){
        printf("字符数组:%s含有非法字符\n",str);
        throw runtime_error("非法字符，转换为数字失败");
    }
    return num;
}

/**
 * 字符串转数字
 * @param str 数字字符串
 * @return 装换完成的数字
 */
int str2int(string str){
    stringstream stream(str);
    int num;
    stream>>num;
    if(!stream.eof()){
        throw runtime_error("数字"+str+"不正确，请检查参数");
    }
    return num;
}

/**
 * 合并count组图片;
 * @param count 合并图片的组数
 * @param m 横向m张图片
 * @param n 纵向n张图片
 * @param paths 指定要合并图片的路径
 * @param savePath 保存路径
 */
void merge(int count,int m,int n,vector<string> paths,string savePath){

    ASSERT(paths.size()<=m*n,"指定的路径数量超过m*n");

    //初始化各个文件夹内指定图片名字的txt文件的输入流
    vector<ifstream> nameStreams;
    for(int i=0;i<paths.size();i++){
        nameStreams.emplace_back(ifstream(paths[i],ios::in));
        if(!nameStreams[i].is_open()){
            throw runtime_error("打开文件\""+paths[i]+"\"异常，请检查该文件是否存在");
        }
    }

    int local_count=0;

    Mat baseMat;//黑色图片
    Mat borderMat;//添加边缘之后的黑色图片

    while (local_count<count){
        local_count++;

        vector<Mat> rows;//
        vector<Mat> cols;//

        // 读取一组图片
        auto size = static_cast<const int>((paths.size()+m-1) / m);//计算这组图片有几行
        for(int j=0;j<m*n;j++){
            string picPath;
            nameStreams[j] >> picPath;
            if(j<paths.size() && !picPath.empty()) {

                //读取图片
                Mat img = imread(picPath);
                ASSERT(!img.empty(), "图片为空,路径："+picPath);

                //初始化baseMat和borderMat
                if(baseMat.empty()){
                    baseMat = Mat(img.size(),CV_8UC3,Scalar(0,0,0));
                    copyMakeBorder(baseMat, borderMat, 30, 5, 5, 5, BORDER_CONSTANT, Scalar(0, 0, 0));
                }

                //判断组内照片是否和baseMat的大小相等
                ASSERT(img.size() == baseMat.size(),"图片大小不同");

                //添加边缘
                copyMakeBorder(img, img, 30, 5, 5, 5, BORDER_CONSTANT, Scalar(0, j*5+50, 0));

                putText(img,to_string(j+1),Point(5,20),FONT_HERSHEY_SIMPLEX,0.4,Scalar(255,255,255));

                cols.push_back(img);
            }else{
                cols.push_back(borderMat);
            }

            // 合并一行
            if (cols.size() == m) {
                Mat one_row;
                hconcat(cols,one_row);
                rows.push_back(one_row);
                cols.clear();
            }

            // 合并完一张图片所需要的行，跳出循环
            if(rows.size()>=size){
                break;
            }
        }

        // 合并列
        Mat final;
        vconcat(rows,final);
        imshow("final",final);
        imwrite(savePath+"/"+to_string(local_count)+".bmp",final);
        waitKey(1);
    }
}