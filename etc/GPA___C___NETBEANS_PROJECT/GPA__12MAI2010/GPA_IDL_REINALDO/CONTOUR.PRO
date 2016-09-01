erase
arquivo = ''
read,'Qual o nome do arquivo: ',arquivo
read,mx,prompt='Qual a dimensao em x: '
read,my,prompt='Qual a dimensao em y: '

v=fltarr(mx,my) 
openr,1,arquivo
readf,1,v
close,1
contour,v,nlevels=10,zrange=[0,200],$
title='Titulo'


end
