;
; --------------------------------------------------------------------------
; 


FUNCTION Calcula_I
  
  COMMON output_window, TOTALVECT, ALIVE_VECT, RETAS, FRAG, MAT_NUMBER, TIME, ENERG
  COMMON DRAW_Comm, DRAW1_Id, DRAW2_Id, DRAW3_Id, DRAW4_Id

  COMMON input_data, totlin, totcol, tol, Sel, x0, x1, y0, y1,submat
  COMMON output_data, nvect, nvetv, soma, tempo, nromat, fa, energia
  COMMON files, name_in, name_out
  COMMON text_sai, texto, ultima_linha
  
  COMMON para_print, z, mx, my, dx, dy, tr, vvx, vvy
  
  fa=0.0
  mx=totlin
  my=totcol
  IF (submat EQ 'Y') THEN BEGIN
    IF ((X1 GE mx) OR (Y1 GE my) OR (x0 GE x1) OR (y0 GE y1)) THEN BEGIN 
       Trata_erro, ERRO=3
       RETURN, -1
    END
  END

  IF ((mx LE 0) OR (my LE 0)) THEN BEGIN
   Trata_erro, ERRO=2
   RETURN, -1
  END

  texto(ultima_linha+1) = ';'   
  texto(ultima_linha) = STRING(FORMAT='("; Arquivo:", A, ", Matriz[", I3,":",I3,",",I3,":",I3,"]",$, ";" )', name_in,x0,x1,y0,y1)
  ultima_linha=ultima_linha+2 
  
  nromat = 0
  GET_LUN, u
  OPENR, U, name_in
  WHILE NOT EOF(u) DO BEGIN

     tempo=SYSTIME(1)
     z=FLTARR(mx,my)
     READF,u,z
     nromat = nromat + 1

     IF submat EQ 'Y' THEN BEGIN
        z=z(x0:x1,y0:y1)
        mx=x1-x0+1
        my=y1-y0+1
     END

     tam = mx*my
     submedia = (median(z)*tam - max(z))/(tam-1)
     deltaenergia = max(z) - submedia

     IF (Sel EQ 'Y') THEN BEGIN
	 WSET, DRAW1_Id
         CONTOUR,z,NLEVELS=8,ZRANGE=[0,max(z)],TITLE='Contour-Pattern', $
         XRANGE=[0,mx], YRANGE=[0,my]         
     ENDIF
         
     dx=gradx(z,mx,my)
     dy=grady(z,mx,my)

; Plotting the total of vectors

     IF (Sel EQ 'Y') THEN BEGIN
	 WSET, DRAW3_Id
	 VELOVECT, dx, dy, DOTS=0
         XYOUTS, 145, 335, 'Gradient Field', CHARSIZE=1.3, /DEVICE
     ENDIF
   
     nvect= N_ELEMENTS(WHERE((dx NE 0) OR (dy NE 0)))

     factor=1/SQRT((ABS(MAX(dx)))^2+(ABS(MAX(dy)))^2)

; Calculates the alive vectors in the Asymmetric Field

     tparcial =  tempo
     nvetv=nvect
     FOR i=0,tam-2 DO BEGIN
	 FOR j=i+1,tam-1 DO BEGIN
	     IF ((dx(i) NE 0) OR (dy(i) NE 0)) THEN BEGIN
		a1=dx(i)+dx(j)
		a2=dy(i)+dy(j)
		r=SQRT(a1^2+a2^2)
		IF (r LE tol) THEN BEGIN
		   dx(i)=0
		   dy(i)=0
		   dx(j)=0
		   dy(j)=0
		   nvetv=nvetv - 2
		ENDIF
            ENDIF
	 ENDFOR
         segs = SYSTIME(1) - tparcial
         IF (segs gt float(mx/2)/5) THEN BEGIN
	     tparcial = SYSTIME(1) - tempo
             str=STRING(FORMAT='(I2.2,":",I2.2,":",I2.2)',tparcial/3600,(tparcial MOD 3600)/60, ((tparcial MOD 3600) MOD 60))
             WIDGET_CONTROL ,TIME, SET_VALUE=str
             tparcial = SYSTIME(1)
         END
     ENDFOR	
     print, i, j

; Plotting the Asymmetric Field

     IF (nvetv NE 0) THEN BEGIN

        IF (Sel EQ "Y") THEN BEGIN
	   WSET, DRAW2_Id
           VELOVECT, dx, dy,dots=0
           XYOUTS, 140, 335, 'Assymetric Field', CHARSIZE=1.3, /DEVICE
        ENDIF

        vvx=FLTARR(nvetv+1)
        vvy=FLTARR(nvetv+1)

	aux=0
	FOR i=0,mx-1 DO BEGIN
	    FOR j=0,my-1 DO BEGIN
		IF ((dx(i,j) NE 0) OR (dy(i,j) NE 0)) THEN BEGIN
			vvx(aux)=i+(dx(i,j)*factor)/2
			vvy(aux)=j+(dy(i,j)*factor)/2
			aux=aux+1
		ENDIF
	    ENDFOR
	ENDFOR

        TRIANGULATE, vvx, vvy, tr, CONNECTIVITY=list
        IF (Sel EQ "Y") THEN BEGIN
	    WSET, DRAW4_Id
	    PLOT,vvx,vvy,psym=1, XRANGE=[min(vvx),max(vvx)], $
            YRANGE=[min(vvy),max(vvy)],  XGRID=1, YGRID=1, TITLE="Triangulation"
	    FOR i=0,N_ELEMENTS(tr)/3-1 DO BEGIN 
	        t = [tr(*,I),tr(0,i)] 
	        PLOTS,vvx(t),vvy(t)
	    ENDFOR
        ENDIF 

        soma=0
        FOR i=0,(N_ELEMENTS(vvx)-1) DO BEGIN 
	    l=list(list(i):list(i+1)-1)
	    FOR j=0,(N_ELEMENTS(l)-1) DO BEGIN
	        IF (i LT l(j)) THEN soma=soma+1
	    ENDFOR
        ENDFOR
        fa = float(soma-nvetv)/nvetv
     ENDIF ELSE BEGIN
        soma=0
        fa=0
     END
     energia = fa * deltaenergia
     tempo = SYSTIME(1) - tempo
     Resultados
     mx=totlin
     my = totcol
   ENDWHILE
   CLOSE, u
   FREE_LUN, u
   RETURN, 0
END


