function GRADY, matriz, lins, cols   
;+
; NAME:
;	GRADIENTE
; PURPOSE:
;	Obter a derivada parcial de uma matriz em X
;
; CALLING SEQUENCE:
;	result = GRADY( matriz, lins, cols)   
; INPUTS:
;
; OUTPUTS:
;
; MODIFICATION HISTORY:
;----------------------------------------------------------------------
 if N_params() LT 3 then begin
     print,'Syntax -     result =  grady( matrix, lins, cols)
     return, -1
 endif 

 dy=fltarr(lins, cols)
 
 x=FLTARR(lins,cols)
   FOR i=0,lins-1 DO BEGIN
	x(*,i)=matriz(*,lins-1-i)	
   ENDFOR

;Derivada Parcial em Y

 FOR i=0,cols-1 DO BEGIN
     z=x(i,*)
     z=deriv(z)
     dy(i,*)=z
 ENDFOR
print,dy
return, dy       

END                 
