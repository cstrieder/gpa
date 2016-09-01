function GRADX, matriz, lins, cols   
;+
; NAME:
;	GRADIENTE
; PURPOSE:
;	Obter a derivada parcial de uma matriz em X
;
; CALLING SEQUENCE:
;	result = GRADX( matriz, lins, cols)   
; INPUTS:
;
; OUTPUTS:
;
; MODIFICATION HISTORY:
;----------------------------------------------------------------------
 if N_params() LT 3 then begin
     print,'Syntax -     result =  gradx( matrix, lins, cols)
     return, -1
 endif 

 dx=fltarr(lins, cols)
 
 x=FLTARR(lins,cols)
   FOR i=0,lins-1 DO BEGIN
	x(*,i)=matriz(*,lins-1-i)	
   ENDFOR

;Derivada Parcial em X

   FOR i=0,lins-1 DO BEGIN
       z=x(*,i)
       z=deriv(z)
       dx(*,i)=z
   ENDFOR
print,dx
return, dx       

END                 
