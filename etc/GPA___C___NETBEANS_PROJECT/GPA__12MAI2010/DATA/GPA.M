function [g1,g2,g3,g4,mi,gamma] = gpa(s,tau,lambda)
% tau = taxa de tolerancia (default 0.0001)
% lambda = vizinhanca (default 1)
% s = escala (default 0)
% Define os valores 'default' para os parametros de entrada
if nargin<3 lambda = 1; end
if nargin<2 tau = 0.0001; end
if nargin<1 s = 0; end

% Abre a janela para o usuario selecionar o arquivo de entrada
% [filename, pathname] = uigetfile({'*.png';'*.gif';'*.jpg';'*.bmp';'*.tif';'*.tiff';'*.jpeg';'*.dat';'*.txt'},'Selecione a matriz');
[filename, pathname] = uigetfile({'*.dat';'*.txt'},'Selecione a matriz');
% Verifica o tipo do arquivo
tipo = filename(end-3:end);
file = strcat(pathname,filename);

switch tipo
    
    % Trata arquivos de dados
    case {'.dat','.txt'}
        
        A = load(file); %carrega o arquivo de dados
        dim = size(A);  %verifica a dimensao dos dados
        if dim(2)==1 || dim(1)==1   %trata 1D
            
            if min(A)<0
                A = A + abs(min(A));
            end
            if s    %trata a opcao de calcular o gpa local (por escala)
                new_s = s^2;
                all_g = [];
                mod(length(A),new_s);
                for i=1:new_s:dim-new_s+1
                    m = converter(A(i:i+new_s-1));
                    all_g = [all_g; momento1(m,tau)];
                end
                figure
                g1 = mean(all_g);
                aux = std(all_g)
                desvio = [g1-aux g1+aux]
                plot(all_g,'-');
                 title('GPA local (3x3) sd52')
                 xlabel('n� janelas')
                 ylabel('g_1^a')
                 save 'g1a16x16_sd52.dat' all_g -ascii
            else
                m = converter(A);
                g1 = momento1(m,tau);
            end

        else    %trata 2D

            if min(min(A))<0
                A = A + abs(min(min(A)));
            end
            if s
                g1 = local_g(A,s,tau);
            else
                g1 = momento1(A,tau);
            end
        end
        
    % Trata arquivos de imagem
    case {'.png','.gif','.jpg','.bmp','.tif','tiff','jpeg'}
        
        I = imread(file);
        info = imfinfo(file);
        if strcmp(info.ColorType,'truecolor')
            m = double(0.299 * I(:,:,1) + 0.5870 * I(:,:,2) + 0.114 * I(:,:,3)); %transforma a imagem de rgb para indexada
        else
            m = double(I);
        end
        if s
            g1 = local_g(m,s,tau);
        else
            g1 = momento1(m,tau);
        end

    % Exibe mensagem de erro caso o arquivo seja de um tipo n�o previsto
    otherwise
        error('Tipo de arquivo n�o reconhecido')
        g1 = 0;
        
end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function m = converter(A)
%Converte uma s�rie de dados em uma matriz retangular aproximada de uma quadrada aproveitando o maior
%numero de pontos possivel

dim = length(A);
aux = [];
l = floor(sqrt(dim));
while mod(dim,l)
    aux = [aux; l mod(dim,l)];
    l = l - 1;
end
c = dim/l;
if l==1
    [ind, mm] = min(aux(:,2));
    l = aux(ind,1);
end
for i=1:l
    m(i,1:c) = A((i-1)*c + 1:i*c);
end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function gl = local_g(m,s,tau)

all_g = [];
[l,c] = size(m);
new_l = floor(l/s);
new_c = floor(c/s);
for i=1:new_l
    for j=1:new_c
        new_m = m((i-1)*s + 1:i*s,(j-1)*s + 1:j*s);
        all_g = [all_g; momento1(new_m,tau)];
    end
end
gl = mean(all_g);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function g = momento1(m,tau)

norm_m = m/max(max(m));
[IX,IY] = gradient(norm_m);

% figure
% image(norm_m)
% quiver(IX,IY)

%calcula o modulo e fase de cada vetor
cont = 1;
for i=1:size(m,1)
    for j=1:size(m,2)
        L(cont,1:2) = [i j];
        L(cont,3) = sqrt(IX(i,j)^2 + IY(i,j)^2);
        L(cont,5) = IX(i,j);
        L(cont,6) = IY(i,j);
        if IY(i,j)==0
            if IX(i,j)<0
                L(cont,4) = pi;
            else
                L(cont,4) = -pi;
            end
        else if IX(i,j)==0
                if IY(i,j)<0
                    L(cont,4) = pi/2;
                else
                    L(cont,4) = -pi/2;
                end
                L(cont,4) = (IY(i,j)/abs(IY(i,j)))*pi/2;
            else L(cont,4) = atan(IY(i,j)/IX(i,j));  %rever!!!
            end
        end
        cont = cont + 1;
    end
end

%4) Percorre L removendo os vetores sim�tricos
 i = 1; j = 2;
 stop = 0;
 while stop~=1
    while stop~=2
        if j<=size(L,1)
            %if (abs(L(j,3)-L(i,3)) <= tau) & (L(j,4) == -1*L(i,4))
            
            if (abs(L(j,3)-L(i,3)) <= tau) & (L(j,5) == -1*L(i,5)) & (L(j,6) == -1*L(i,6))
                L(j,:) = [];
                L(i,:) = [];
                j = i + 1;
            else
                j = j + 1;
            end
        else
            stop = 2;
        end
    end
    if i<size(L,1)
        i = i + 1;
        j = i + 1;
        stop = 0;
    else
        stop = 1;
    end
 end
 
 %Verifica se sobrou um vetor nulo
 if size(L,1)==1 & L(1,3)==0
     L = [];
 else
 
 %figure
 %colormap(gray)
 %image(m) 
 %hold on 
 %if L
 %  plot(L(:,1),L(:,2),'.b')
 %end
 
 lados = 0;

     L(:,7:8) = (L(:,2:-1:1) + L(:,5:6));
     X = L(:,7);
     Y = L(:,8);
%      X = [0, -0.416, -1.35, -1.64, -1.31, -0.532, 0.454, 1.45, 2.29, 2.88, 3.16, 3.12, 2.77, 2.16, 1.36];
%      Y = [0, 0.909, 0.436, -0.549, -1.51, -2.17, -2.41, -2.21, -1.66, -0.838, -0.131, 1.14, 2.08, 2.89, 3.49];
     tri = delaunay(X,Y);
%      tri = delaunay(L(:,7),L(:,8));
     %figure
     %figure
     %tri2 = delaunay(L(:,1),L(:,2));
     %triplot(tri2,L(:,1),L(:,2),'r')
     
     lados = size(tri,1)*3;
     % Calcula o numero de lados
     i = 1; ii = 2;
     stop = 0;
     comum = 0;
     while stop~=1
         while stop~=2
             if ii<=size(tri,1)
                 comum = (tri(ii,1)==tri(i,1) || (tri(ii,1)==tri(i,2)) || (tri(ii,1)==tri(i,3))) + (tri(ii,2)==tri(i,1) || (tri(ii,2)==tri(i,2)) || (tri(ii,2)==tri(i,3))) + (tri(ii,3)==tri(i,1) || (tri(ii,3)==tri(i,2)) || (tri(ii,3)==tri(i,3))) - 1;
                 if comum>0
                     lados = lados - comum;
                 end
                 ii = ii + 1;
             else
                 stop = 2;
             end
         end
         if i<size(tri,1)
             i = i + 1;
             ii = i + 1;
             stop = 0;
         else
             stop = 1;
         end
     end
 
 end
 
 if size(L,1)>2
     %plotar(norm_m,IX,IY,L,tri);
%      disp('Lados')
%      disp(lados)
%      disp('Vetores Assim�tricos')
%      disp(size(L,1))
        a = (lados-size(L,1));
        b = size(L,1);
     g = (lados-size(L,1))/size(L,1);
 else
     g = 0;
 end
 
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function plotar(norm_m,IX,IY,L,tri)

figure
image(norm_m)
quiver(IX,IY)
hold on
triplot(tri,L(:,7),L(:,8),'r')
