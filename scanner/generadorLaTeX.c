#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void generar_beamer_variables(int cantidades[], int lineas_errores[]) {
    FILE *f = fopen("prueba.tex", "w");
    if (!f) {
        perror("Error al crear prueba.tex");
        return;
    }
    int keyboard = 0, operadores = 0, literales = 0, errores = 0;
    int i;
    for (i = 0; i < 44; i++) keyboard =+ cantidades[i];
    for (; i<78; i++) operadores =+ cantidades[i];
    for (; i < 84; i++) literales =+ cantidades[i];
    errores = cantidades[84]+cantidades[85];

    fprintf(f,
        "\\documentclass{beamer}\n"
    "\\usepackage{url}\n"
    "\\usepackage[utf8]{inputenc}\n"
    "\\usepackage{comment}\n"
    "\\usepackage{pgf-pie}\n"
    "\\usepackage{multirow}\n"
    "\\usepackage{colortbl}\n"
    "\\usepackage{xcolor}\n"
    "\\usetheme{Madrid}\n"
    "\\usefonttheme{serif}\n"
    "\\usecolortheme{rose}\n\n"

    "\\newcommand{\\keyword}{%d}\n"
    "\\newcommand{\\operadoresysimbolos}{%d}\n"
    "\\newcommand{\\literaleseidentificadores}{%d}\n"
    "\\newcommand{\\erroresyfindearchivo}{%d}\n\n"

    "\\newcommand{\\tokauto}{%d}\n"
    "\\newcommand{\\tokbreak}{%d}\n"
    "\\newcommand{\\tokcase}{%d}\n"
    "\\newcommand{\\tokchar}{%d}\n"
    "\\newcommand{\\tokconst}{%d}\n"
    "\\newcommand{\\tokcontinue}{%d}\n"
    "\\newcommand{\\tokdefault}{%d}\n"
    "\\newcommand{\\tokdo}{%d}\n"
    "\\newcommand{\\tokdouble}{%d}\n"
    "\\newcommand{\\tokelse}{%d}\n"
    "\\newcommand{\\tokenum}{%d}\n"
    "\\newcommand{\\tokextern}{%d}\n"
    "\\newcommand{\\tokfloat}{%d}\n"
    "\\newcommand{\\tokfor}{%d}\n"
    "\\newcommand{\\tokgoto}{%d}\n"
    "\\newcommand{\\tokif}{%d}\n"
    "\\newcommand{\\tokint}{%d}\n"
    "\\newcommand{\\toklong}{%d}\n"
    "\\newcommand{\\tokregister}{%d}\n"
    "\\newcommand{\\tokreturn}{%d}\n"
    "\\newcommand{\\tokshort}{%d}\n"
    "\\newcommand{\\toksigned}{%d}\n"
    "\\newcommand{\\toksizeof}{%d}\n"
    "\\newcommand{\\tokstatic}{%d}\n"
    "\\newcommand{\\tokstruct}{%d}\n"
    "\\newcommand{\\tokswitch}{%d}\n"
    "\\newcommand{\\toktypedef}{%d}\n"
    "\\newcommand{\\tokunion}{%d}\n"
    "\\newcommand{\\tokunsigned}{%d}\n"
    "\\newcommand{\\tokvoid}{%d}\n"
    "\\newcommand{\\tokvolatile}{%d}\n"
    "\\newcommand{\\tokwhile}{%d}\n"
    "\\newcommand{\\toklonglong}{%d}\n"
    "\\newcommand{\\toklongdouble}{%d}\n"
    "\\newcommand{\\tokbool}{%d}\n"
    "\\newcommand{\\toktrue}{%d}\n"
    "\\newcommand{\\tokfalse}{%d}\n"
    "\\newcommand{\\tokrestrict}{%d}\n"
    "\\newcommand{\\tokinline}{%d}\n"
    "\\newcommand{\\tokcomplex}{%d}\n"
    "\\newcommand{\\tokimaginary}{%d}\n"
    "\\newcommand{\\tokthreadlocal}{%d}\n"
    "\\newcommand{\\tokatomic}{%d}\n"
    "\\newcommand{\\toknoreturn}{%d}\n\n"

    "\\newcommand{\\tokplus}{%d}\n"
    "\\newcommand{\\tokminus}{%d}\n"
    "\\newcommand{\\tokmult}{%d}\n"
    "\\newcommand{\\tokdiv}{%d}\n"
    "\\newcommand{\\tokassign}{%d}\n"
    "\\newcommand{\\tokeq}{%d}\n"
    "\\newcommand{\\tokneq}{%d}\n"
    "\\newcommand{\\toklt}{%d}\n"
    "\\newcommand{\\tokgt}{%d}\n"
    "\\newcommand{\\tokleq}{%d}\n"
    "\\newcommand{\\tokgeq}{%d}\n"
    "\\newcommand{\\tokand}{%d}\n"
    "\\newcommand{\\tokor}{%d}\n"
    "\\newcommand{\\toknot}{%d}\n"
    "\\newcommand{\\tokbitand}{%d}\n"
    "\\newcommand{\\tokbitor}{%d}\n"
    "\\newcommand{\\tokxor}{%d}\n"
    "\\newcommand{\\tokcomplement}{%d}\n"
    "\\newcommand{\\toklshift}{%d}\n"
    "\\newcommand{\\tokrshift}{%d}\n"
    "\\newcommand{\\tokincrement}{%d}\n"
    "\\newcommand{\\tokdecrement}{%d}\n"
    "\\newcommand{\\tokdot}{%d}\n"
    "\\newcommand{\\tokarrow}{%d}\n"
    "\\newcommand{\\toklparen}{%d}\n"
    "\\newcommand{\\tokrparen}{%d}\n"
    "\\newcommand{\\toklbrace}{%d}\n"
    "\\newcommand{\\tokrbrace}{%d}\n"
    "\\newcommand{\\toklbracket}{%d}\n"
    "\\newcommand{\\tokrbracket}{%d}\n"
    "\\newcommand{\\toksemicolon}{%d}\n"
    "\\newcommand{\\tokcolon}{%d}\n"
    "\\newcommand{\\tokcomma}{%d}\n"
    "\\newcommand{\\tokellipsis}{%d}\n\n"

    "\\newcommand{\\tokidentifier}{%d}\n"
    "\\newcommand{\\tokintliteral}{%d}\n"
    "\\newcommand{\\tokfloatliteral}{%d}\n"
    "\\newcommand{\\tokstringliteral}{%d}\n"
    "\\newcommand{\\tokcharliteral}{%d}\n"
    "\\newcommand{\\tokboolliteral}{%d}\n\n"

    "\\newcommand{\\tokerror}{%d}\n"
    "\\newcommand{\\tokeof}{%d}\n\n"

    "\\usepackage{listings}\n"
    "\\usepackage{xcolor}\n"
    "\\usepackage{graphicx}\n\n"

    "\\definecolor{codegreen}{rgb}{0,0.6,0}\n"
    "\\definecolor{codegray}{rgb}{0.5,0.5,0.5}\n"
    "\\definecolor{codepurple}{rgb}{0.58,0,0.82}\n"
    "\\definecolor{backcolour}{rgb}{0.95,0.95,0.92}\n\n"

    "\\lstdefinestyle{mycstyle}{\n"
    "    language=C,\n"
    "    backgroundcolor=\\color{backcolour},\n"
    "    moredelim=[is][\\color{red}\\bfseries]{|}{|},"
    "    commentstyle=\\color{codegreen},\n"
    "    keywordstyle=\\color{magenta},\n"
    "    numberstyle=\\tiny\\color{codegray},\n"
    "    stringstyle=\\color{codepurple},\n"
    "    basicstyle=\\ttfamily\\scriptsize,\n"
    "    breakatwhitespace=false,\n"
    "    breaklines=true,\n"
    "    captionpos=b,\n"
    "    keepspaces=true,\n"
    "    numbers=left,\n"
    "    numbersep=5pt,\n"
    "    showspaces=false,\n"
    "    showstringspaces=false,\n"
    "    showtabs=false,\n"
    "    tabsize=2,\n"
    "    frame=single\n"
    "}\n\n"

    "\\title[Scaner]{Scaner}\n"
    "\\author[Caleb\\and Sebastián \\and Desireé]{Caleb Alfaro \\and Sebastián Quesada \\and Desireé Avilés\\inst{1}}\n"
    "\\date[TEC 2025]{Segundo Proyecto de Compiladores, Mayo 2025}\n\n"

    "\\logo{\\includegraphics[height=0.5cm]{logoTEC.png}}\n\n"

    "\\begin{document}\n"
    "\\begin{frame}\n"
    "  \\centering\n"
    "  \\vspace*{0.3cm}\n"
    "  \\includegraphics[scale = 0.07]{logoTEC.png}\\\\\n"
    "  \\textsc{\\small Instituto Tecnológico de Costa Rica}\\\\[1.7em]\n"
    "  \\textsc{\\small Compiladores e intérpretes}\\\\[0.3em]\n"
    "  \\textsc{\\small Segundo Proyecto}\\\\[0.5em]\n"
    "  \\rule{\\linewidth}{0.2 mm} \\\\[0.3 cm]\n"
    "  {\\LARGE \\textbf{Scanner}}\\\\\n"
    "  \\rule{\\linewidth}{0.2 mm} \\\\[0.3 cm]\n"
    "  {\\normalsize \\emph{Caleb Alfaro, Sebastián Quesada, Desireé Avilés } }\\\\[1.6em]\n"
    "  {\\small \\emph{Profesor/a:} Eddy Miguel Ramírez Jiménez}\\\\[0.5em]\n"
    "  {\\footnotesize \\today}\n"
    "\\end{frame}\n\n"
    "\\begin{frame}{Proceso general de \\textit{scanning}}\n"
    "\\begin{itemize}\n"
    "    \\item Para la etapa de preproceso se procesa el archivo fuente de forma modular, (archivo por archivo, línea por línea, char por char). Es importante destacar que en el momento en el que se procesa un  \\#include, se empieza a procesar inmediatamente ese archivo, y se añade al preprocesado de salida. Esto para que no solo se mantenga el orden esperado por el usuario, sino también para que la lógica de procesos para los \\#define, tenga el comportamiento esperado (que una vez definido, influya en todas las líneas por debajo de este).\n\n"
    "    \\item Los comentarios se eliminan por completo para el archivo preprocesado salida, preprocesando la línea previo al procesamiento general. Los \\#defines se aplican al procesar una línea, verificando si existen apariciones y remplazando por su valor, si fuera el caso.\n"
    "\\end{itemize}\n"
    "\\end{frame}\n\n"
    "\\begin{frame}{Herramienta flex}\n"
    "Flex es una herramienta generadora de analizadores léxicos, también conocidos como escáneres. Nos facilita la creación de programas que reconocen patrones léxicos en texto, utilizando expresiones regulares. Flex toma una descripción de un analizador léxico, expresada en pares de expresiones regulares y código C (reglas), y genera un archivo fuente en C que implementa el analizador.\n"
    "\\end{frame}\n\n"

    "\\begin{frame}{Herramienta flex}\n"
    "\\begin{enumerate}\n"
    " \\item<1-> \\textbf{Definición de reglas:} El usuario define un conjunto de reglas, donde cada regla consiste en una expresión regular y una acción en C. La expresión regular describe el patrón a buscar en el texto de entrada, y la acción en C especifica qué hacer cuando se encuentra ese patrón.\n"
    " \\item<2-> \\textbf{Generación de código C:} Flex toma estas reglas y genera un archivo fuente en C que contiene la implementación del analizador léxico. Este archivo contiene la lógica para encontrar y procesar los patrones definidos en las reglas.\n"
    " \\item<3> \\textbf{Compilación y uso:} El archivo fuente en C generado por Flex se compila con un compilador de C y se utiliza para crear un programa que realiza la tarea de analizar léxicamente el texto de entrada.\n"
    "\\end{enumerate}\n"
    "\\end{frame}\n\n",
    keyboard, operadores , literales , errores,
    cantidades[0],
    cantidades[1],
    cantidades[2],
    cantidades[3],
    cantidades[4],
    cantidades[5],
    cantidades[6],
    cantidades[7],
    cantidades[8],
    cantidades[9],
    cantidades[10],
    cantidades[11],
    cantidades[12],
    cantidades[13],
    cantidades[14],
    cantidades[15],
    cantidades[16],
    cantidades[17],
    cantidades[18],
    cantidades[19],
    cantidades[20],
    cantidades[21],
    cantidades[22],
    cantidades[23],
    cantidades[24],
    cantidades[25],
    cantidades[26],
    cantidades[27],
    cantidades[28],
    cantidades[29],
    cantidades[30],
    cantidades[31],
    cantidades[32],
    cantidades[33],
    cantidades[34],
    cantidades[35],
    cantidades[36],
    cantidades[37],
    cantidades[38],
    cantidades[39],
    cantidades[40],
    cantidades[41],
    cantidades[42],
    cantidades[43],
    cantidades[44],
    cantidades[45],
    cantidades[46],
    cantidades[47],
    cantidades[48],
    cantidades[49],
    cantidades[50],
    cantidades[51],
    cantidades[52],
    cantidades[53],
    cantidades[54],
    cantidades[55],
    cantidades[56],
    cantidades[57],
    cantidades[58],
    cantidades[59],
    cantidades[60],
    cantidades[61],
    cantidades[62],
    cantidades[63],
    cantidades[64],
    cantidades[65],
    cantidades[66],
    cantidades[67],
    cantidades[68],
    cantidades[69],
    cantidades[70],
    cantidades[71],
    cantidades[72],
    cantidades[73],
    cantidades[74],
    cantidades[75],
    cantidades[76],
    cantidades[77],
    cantidades[78],
    cantidades[79],
    cantidades[80],
    cantidades[81],
    cantidades[82],
    cantidades[83],
    cantidades[84],
    cantidades[85]
    );
    //ahora lo que sigue es escribir el codigo de c aquí y luego en el latex

    FILE *f_prepro = fopen("preprocesado.c", "r");
    if (!f_prepro) {
        perror("Error al abrir preprocesado.c");
        fclose(f);
        exit(1);
    }
    fprintf(f,
            "\\begin{frame}[fragile]{Programa después del preproceso}{}\n\n"
            "  \\begin{lstlisting}[style=mycstyle]\n"
        );

    char buffer[1024];
    int linea = 0, idx_error = 0;

    while (linea < 92 && fgets(buffer, 1024, f_prepro)) {
        if (idx_error < cantidades[84] && linea+1==lineas_errores[idx_error]) {
            fprintf(f, "|");
        }
        fputs(buffer, f);
        if (idx_error < cantidades[84] && linea+1==lineas_errores[idx_error]) {
            fprintf(f, "|");
            idx_error++;
        }
        linea++;
        if (linea%23==0) {
            fprintf(f,
        "  \\end{lstlisting}\n"
                "\\end{frame}\n\n"
                );
            fprintf(f,
               "\\begin{frame}[fragile]{Programa después del preproceso}{}\n\n"
                "  \\begin{lstlisting}[style=mycstyle]\n"
           );
        }
    }



    fclose(f_prepro);
    fprintf(f,
        "  \\end{lstlisting}\n"
        "\\end{frame}\n\n"
    );

    /*AQUI TERMINA LO DE ESCRIBIR EL FILE DEL PREPROCESO FUNCIONA :> */

    fprintf(f,"\\begin{frame}{Histograma de las cantidades de tokens}\n"
    "\\begin{table}\n"
    "\\begin{tabular}{p{5cm}|>{\\centering\\arraybackslash}p{3cm}}\n"
    "         \\rowcolor{blue!30}&\\\\\n"
    "         \\rowcolor{blue!30} \\centering{\\large \\textbf{Categoría Léxica}} &  {\\large \\textbf{Cantidad}}\\\\\n"
    "         \\rowcolor{blue!30}&\\\\\n"
    "         \\arrayrulecolor{blue!30}\n"
    "         \\hline\n"
    "         Palabras Reservadas & \\keyword\\\\\n"
    "         \\hline\n"
    "         Operadores y símbolos  & \\operadoresysimbolos\\\\\n"
    "         \\hline\n"
    "         Literales e identificadores &\\literaleseidentificadores \\\\\n"
    "         \\hline\n"
    "         Errores y fin de archivo &\\erroresyfindearchivo\\\\\n"
    "         \\hline\n"
    "         \\end{tabular}\n"
    "\\end{table}\n"
    "\\end{frame}\n\n"

    "\\begin{frame}{Cantidades de palabras reservadas}\n"
    "\\begin{table}[h]\n"
    "\\centering\n"
    "\\begin{tabular}{p{2cm}|>{\\centering\\arraybackslash}p{1.6cm}\n"
    "                |p{2cm}|>{\\centering\\arraybackslash}p{1.6cm}}\n"
    "         \\arrayrulecolor{blue!30}\n"
    "        \\rowcolor{blue!30} &&&\\\\\n"
    "         \\rowcolor{blue!30} \n"
    "                \\centering{\\textbf{Palabra}} &  \n"
    "                { \\textbf{Cantidad}} &\n"
    "                \\centering{\\textbf{Palabra}} &  \n"
    "                { \\textbf{Cantidad}} \\\\\n"
    "        \\rowcolor{blue!30} &&&\\\\\n"
    "        \\hline\n"
    "        break & \\tokbreak & case & \\tokcase\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3} char & \\tokchar & const & \\tokconst\\\\\n"
    "        \\hline\n"
    "        continue & \\tokcontinue & default & \\tokdefault\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3} do & \\tokdo & double & \\tokdouble\\\\\n"
    "        \\hline\n"
    "        else & \\tokelse & enum & \\tokenum\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3} extern & \\tokextern & float & \\tokfloat\\\\\n"
    "        \\hline\n"
    "        for & \\tokfor & goto & \\tokgoto\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3} if & \\tokif & int & \\tokint\\\\\n"
    "        \\hline\n"
    "        long & \\toklong & register & \\tokregister\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3} return & \\tokreturn & short & \\tokshort\\\\\n"
    "        \\hline\n"
    "        signed & \\toksigned & sizeof & \\toksizeof\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3} static & \\tokstatic & struct & \\tokstruct\\\\\n"
    "        \\hline\n"
    "\\end{tabular}\n"
    "\\end{table}\n"
    "\\end{frame}\n\n"

    "\\begin{frame}{Cantidades de palabras reservadas}\n"
    "\\begin{table}[h]\n"
    "\\centering\n"
    "\\begin{tabular}{p{2cm}|>{\\centering\\arraybackslash}p{1.6cm}\n"
    "                |p{2cm}|>{\\centering\\arraybackslash}p{1.6cm}}\n"
    "         \\arrayrulecolor{blue!30}\n"
    "        \\rowcolor{blue!30} &&&\\\\\n"
    "         \\rowcolor{blue!30} \n"
    "                \\centering{\\textbf{Palabra}} &  \n"
    "                { \\textbf{Cantidad}} &\n"
    "                \\centering{\\textbf{Palabra}} &  \n"
    "                { \\textbf{Cantidad}} \\\\\n"
    "        \\rowcolor{blue!30} &&&\\\\\n"
    "        \\hline        \n"
    "        switch & \\tokswitch & typedef & \\toktypedef\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3} union & \\tokunion & unsigned & \\tokunsigned\\\\\n"
    "        \\hline\n"
    "        void & \\tokvoid & volatile & \\tokvolatile\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3} while & \\tokwhile & long long & \\toklonglong\\\\\n"
    "        \\hline\n"
    "        long double & \\toklongdouble & bool & \\tokbool\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3} true & \\toktrue & false & \\tokfalse\\\\\n"
    "        \\hline\n"
    "        restrict & \\tokrestrict & inline & \\tokinline\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3} complex & \\tokcomplex & imaginary & \\tokimaginary\\\\\n"
    "        \\hline\n"
    "        thread\\_local & \\tokthreadlocal & atomic & \\tokatomic\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3} noreturn & \\toknoreturn & & \\\\\n"
    "        \\hline\n"
    "\\end{tabular}\n"
    "\\end{table}\n"
    "\\end{frame}\n\n"

    "\\begin{frame}{Cantidades de los operadores y símbolos}\n"
    "\\begin{table}[h]\n"
    "\\centering\n"
    "\\begin{tabular}{p{2cm}|>{\\centering\\arraybackslash}p{1.6cm}\n"
    "                |p{2cm}|>{\\centering\\arraybackslash}p{1.6cm}}\n"
    "         \\arrayrulecolor{blue!30}\n"
    "        \\rowcolor{blue!30} &&&\\\\\n"
    "         \\rowcolor{blue!30} \n"
    "                \\centering{\\textbf{Palabra}} &  \n"
    "                { \\textbf{Cantidad}} &\n"
    "                \\centering{\\textbf{Palabra}} &  \n"
    "                { \\textbf{Cantidad}} \\\\\n"
    "        \\rowcolor{blue!30} &&&\\\\\n"
    "        \\hline\n"
    "        plus & \\tokplus & minus & \\tokminus\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3} mult & \\tokmult & div & \\tokdiv\\\\\n"
    "        \\hline\n"
    "        assign & \\tokassign & eq & \\tokeq\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3} neq & \\tokneq & lt & \\toklt\\\\\n"
    "        \\hline\n"
    "        gt & \\tokgt & leq & \\tokleq\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3} geq & \\tokgeq & and & \\tokand\\\\\n"
    "        \\hline\n"
    "        or & \\tokor & not & \\toknot\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3} bitand & \\tokbitand & bitor & \\tokbitor\\\\\n"
    "        \\hline\n"
    "        xor & \\tokxor & complement & \\tokcomplement\\\\\n"
    "        \\hline\n"
    "\\end{tabular}\n"
    "\\end{table}\n"
    "\\end{frame}\n\n"

    "\\begin{frame}{Cantidades de los operadores y símbolos}\n"
    "\\begin{table}[h]\n"
    "\\centering\n"
    "\\begin{tabular}{p{2cm}|>{\\centering\\arraybackslash}p{1.6cm}\n"
    "                |p{2cm}|>{\\centering\\arraybackslash}p{1.6cm}}\n"
    "         \\arrayrulecolor{blue!30}\n"
    "        \\rowcolor{blue!30} &&&\\\\\n"
    "         \\rowcolor{blue!30} \n"
    "                \\centering{\\textbf{Palabra}} &  \n"
    "                { \\textbf{Cantidad}} &\n"
    "                \\centering{\\textbf{Palabra}} &  \n"
    "                { \\textbf{Cantidad}} \\\\\n"
    "        \\rowcolor{blue!30} &&&\\\\\n"
    "        \\hline  \n"
    "        \\rowcolor{blue!3} lshift & \\toklshift & rshift & \\tokrshift\\\\\n"
    "        \\hline\n"
    "        increment & \\tokincrement & decrement & \\tokdecrement\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3} dot & \\tokdot & arrow & \\tokarrow\\\\\n"
    "        \\hline\n"
    "        lparen & \\toklparen & rparen & \\tokrparen\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3} lbrace & \\toklbrace & rbrace & \\tokrbrace\\\\\n"
    "        \\hline\n"
    "        lbracket & \\toklbracket & rbracket & \\tokrbracket\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3} semicolon & \\toksemicolon & colon & \\tokcolon\\\\\n"
    "        \\hline\n"
    "        comma & \\tokcomma & ellipsis & \\tokellipsis\\\\\n"
    "        \\hline\n"
    "\\end{tabular}\n"
    "\\end{table}\n"
    "\\end{frame}\n\n"

    "\\begin{frame}{Cantidades de literales e identificadores, errores y final de archivo}\n"
    "\\begin{table}[h]\n"
    "\\centering\n"
    "\\begin{tabular}{p{3cm}|>{\\centering\\arraybackslash}p{1.8cm}}\n"
    "         \\arrayrulecolor{blue!30}\n"
    "        \\rowcolor{blue!30} &\\\\\n"
    "         \\rowcolor{blue!30} \n"
    "                \\centering{\\textbf{Palabra}} &  \n"
    "                { \\textbf{Cantidad}}\\\\\n"
    "        \\rowcolor{blue!30} &\\\\\n"
    "        identifier & \\tokidentifier\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3}intliteral & \\tokintliteral\\\\\n"
    "        \\hline\n"
    "        floatliteral & \\tokfloatliteral\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3}stringliteral & \\tokstringliteral\\\\\n"
    "        \\hline\n"
    "        error & \\tokerror\\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3}eof & \\tokeof\\\\\n"
    "        \\hline\n"
    "         char literal &\\tokcharliteral \\\\\n"
    "        \\hline\n"
    "        \\rowcolor{blue!3} bool literal & \\tokboolliteral\\\\\n"
    "        \\hline\n"
    "\\end{tabular}\n"
    "\\end{table}\n"
    "\\end{frame}\n\n"

    "\\begin{frame}{Grafico de pastel}{Cantidad de cada categoría léxica}\n"
    "\\centering\n"
    "\\begin{tikzpicture}\n"
    "    \\pie[\n"
    "        sum=auto,\n"
    "        text=legend\n"
    "    ]\n"
    "    {\\keyword/Palabras Reservadas, \\operadoresysimbolos/Operadores y símbolos, \\literaleseidentificadores/Literales e identificadores, \\erroresyfindearchivo/Errores y fin de archivo}\n"
    "\\end{tikzpicture}\n"
    "\\end{frame}\n\n"

    "\\begin{frame}{Bibliografía y fuentes consultadas}\n"
    "\\begin{thebibliography}{9}\n\n"
    "\\bibitem{introflexbison}\n"
    "Universidad de Zaragoza. (2003-2004). \\emph{Introducción a Flex y Bison}. Departamento de Informática e Ingeniería de Sistemas. \\\\\n"
    "\\url{https://webdiis.unizar.es/asignaturas/LGA/material_2003_2004/Intro_Flex_Bison.pdf}\n\n"
    "\\bibitem{mitflex}\n"
    "Free Software Foundation. (s.f.). \\emph{Flex: The Fast Lexical Analyzer}. Massachusetts Institute of Technology. (Trabajo original en inglés, traducido al español). \\\\\n"
    "\\url{https://web-mit-edu.translate.goog/gnu/doc/html/flex_1.html?_x_tr_sl=en&_x_tr_tl=es&_x_tr_hl=es&_x_tr_pto=sge}\n\n"
    "\\end{thebibliography}\n"
    "\\end{frame}\n\n"

    "\\end{document}\n");

    fclose(f);

}

void generar_beamer(int arreglo_cantidades[]) {
    int errores[] = {4, 32};
    generar_beamer_variables(arreglo_cantidades, errores);
    // 2. Compilar el archivo .tex de manera silenciosa
    if (system("pdflatex -interaction=nonstopmode prueba.tex > /dev/null 2>&1") != 0) {
        fprintf(stderr, "Error al compilar prueba.tex\n");
        exit(1);
    }

    // 3. Abrir el PDF inmediatamente en modo pantalla completa
    if (system("mupdf -r 120 prueba.pdf &") != 0) {
        fprintf(stderr, "Error al abrir prueba.pdf con mupdf\n");
        exit(1);
    }
}

// int main() {
//
//     int arreglo_prueba[] = {1, 500, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85};
//     generar_beamer(arreglo_prueba);
//
//     return 0;
// }
