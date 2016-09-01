PRO Trata_erro, erro=err

  COMMON output_window, TOTALVECT, ALIVE_VECT, RETAS, FRAG , MAT_NUMBER, TIME, ENERG, SLIDER2
  COMMON message_window, MESSAGE

Tabela_erros = ["Arquivo nao encontrado.", $
            "Tolerancia deve ser maior ou igual que zero.", $
                "Matriz deve ter pelo menos 1 elemento. ",$
       "Indice da submatriz fora dos limites. ", $
                "Arquivo possui mais que uma matriz, processando todas ...", $
                "Erro na leitura do arquivo: numero de dados insuficiente"]
WIDGET_CONTROL, MESSAGE, SET_VALUE=Tabela_erros(err)
END

;
; ----------------------------------------------------------------------
;

PRO Resultados

  COMMON output_window, TOTALVECT, ALIVE_VECT, RETAS, FRAG, MAT_NUMBER, TIME, ENERG, SLIDER2
  COMMON message_window, MESSAGE

  COMMON output_data, nvect, nvetv, soma, tempo, nromat, fa, energia
  COMMON text_sai, texto, ultima_linha

  str=STRING(FORMAT='(I2.2,":",I2.2,":",I2.2)',tempo/3600,(tempo MOD    3600)/60, ((tempo MOD 3600) MOD 60))
  WIDGET_CONTROL ,TIME, SET_VALUE=str
  WIDGET_CONTROL ,TOTALVECT, SET_VALUE=nvect
  WIDGET_CONTROL ,ALIVE_VECT, SET_VALUE=nvetv
  WIDGET_CONTROL ,RETAS, SET_VALUE=soma
  WIDGET_CONTROL, MAT_NUMBER, SET_VALUE=nromat
  WIDGET_CONTROL ,FRAG, SET_VALUE=fa
  WIDGET_CONTROL ,ENERG, SET_VALUE=energia

  texto(ultima_linha) = STRING(FORMAT='(I6," ", I," ",I," ",I," ",F10.5," ",F10.5)',nromat, nvect, nvetv,soma,fa,energia)
  ultima_linha = ultima_linha+1

END

;
; ---------------------------------------------------------------------------
;

PRO Salva, nome

  COMMON message_window, MESSAGE

  COMMON files, name_in, name_out
  COMMON text_sai, texto, ultima_linha

  GET_LUN, un
  OPENW, un, name_out, ERROR=erro

  IF (erro NE 0) THEN BEGIN
     str='Arquivo de saida nao especificado. '
     WIDGET_CONTROL, MESSAGE, SET_VALUE=str
  ENDIF ELSE BEGIN
      texto(0) = STRING(";       **** Calculo da Fragmentacao ****")
      texto(1) = ";"
      texto(2) = STRING(FORMAT='("; Sessao: ",A)',SYSTIME(0))
      texto(3) = ";"

      FOR l=0,ultima_linha DO BEGIN
          PRINTF, un, texto(l)
      END
      CLOSE, un
      FREE_LUN, un
      ultima_linha = 4
      str='Sessao salva no arquivo ' + name_out + '!'
      WIDGET_CONTROL, MESSAGE, SET_VALUE=str
  END
END

;
; ---------------------------------------------------------------------------
;

PRO Initialise

  COMMON output_window, TOTALVECT, ALIVE_VECT, RETAS, FRAG, MAT_NUMBER, TIME, ENERG, SLIDER2
  COMMON area_draw, DESENHO, DRAW1, DRAW2, DRAW3, DRAW4, EXISTE
  COMMON DRAW_Comm, DRAW1_Id, DRAW2_Id, DRAW3_Id, DRAW4_Id
  COMMON message_window, MESSAGE

  COMMON input_data, totlin, totcol, tol, Sel, x0, x1, y0, y1,submat
  COMMON files, name_in, name_out

  IF (Sel EQ 'Y') THEN BEGIN
    IF (EXISTE EQ 'N')  THEN BEGIN
         WIDGET_CONTROL, DESENHO, /REALIZE
     WIDGET_CONTROL, DRAW1, GET_VALUE=DRAW1_Id
     WIDGET_CONTROL, DRAW2, GET_VALUE=DRAW2_Id
     WIDGET_CONTROL, DRAW3, GET_VALUE=DRAW3_Id
     WIDGET_CONTROL, DRAW4, GET_VALUE=DRAW4_Id
    EXISTE = 'S'
     ENDIF ELSE BEGIN
         WSET, DRAW1_Id
         ERASE
         WSET, DRAW2_Id
         ERASE
         WSET, DRAW3_Id
         ERASE
         WSET, DRAW4_Id
         ERASE
     END
   END
   WIDGET_CONTROL, TOTALVECT, SET_VALUE=' '
   WIDGET_CONTROL, ALIVE_VECT, SET_VALUE=' '
   WIDGET_CONTROL, RETAS, SET_VALUE=' '
   WIDGET_CONTROL, FRAG, SET_VALUE=' '
   WIDGET_CONTROL, ENERG, SET_VALUE=' '
   WIDGET_CONTROL ,TIME, SET_VALUE='0:00:00'
   WIDGET_CONTROL ,MESSAGE, SET_VALUE='       '
   WIDGET_CONTROL ,SLIDER2, SET_VALUE=0
   str='Processando file '+ name_in + '...'
   WIDGET_CONTROL, MESSAGE, SET_VALUE=str
END


;
; --------------------------------------------------------------------------
;


FUNCTION Calcula_I

  COMMON output_window, TOTALVECT, ALIVE_VECT, RETAS, FRAG, MAT_NUMBER, TIME, ENERG, SLIDER2
  COMMON DRAW_Comm, DRAW1_Id, DRAW2_Id, DRAW3_Id, DRAW4_Id

  COMMON input_data, totlin, totcol, tol, Sel, x0, x1, y0, y1,submat
  COMMON output_data, nvect, nvetv, soma, tempo, nromat, fa, energia
  COMMON files, name_in, name_out
  COMMON text_sai, texto, ultima_linha

  COMMON para_print, z, mx, my, dx, dy, tr, vvx, vvy

  fa=0.0
  mx=totlin
  my=totcol

  IF ((mx LE 0) OR (my LE 0)) THEN RETURN, 2

  IF (submat EQ 'Y') THEN BEGIN
    IF ((X1 GE mx) OR (Y1 GE my) OR (x0 GE x1) OR (y0 GE y1)) THEN $
       RETURN, 3
  END

  texto(ultima_linha+1) = ';'
  texto(ultima_linha) = STRING(FORMAT='("; Arquivo:", A, ", Matriz[", I6,":",I6,",",I6,":",I6,"]",$, ";" )', name_in,x0,x1,y0,y1)
  ultima_linha=ultima_linha+2

  nromat = 0
  GET_LUN, u
  OPENR, U, name_in
  WHILE NOT EOF(u) DO BEGIN

     tempo=SYSTIME(1)
     z=FLTARR(mx,my)
     ok = 0
     ON_IOERROR, E_read
     READF,u,z
     ok = 1
     nromat = nromat + 1

     IF submat EQ 'Y' THEN BEGIN
        z=z(x0:x1,y0:y1)
        mx=x1-x0+1
        my=y1-y0+1
     END

     tam = mx*my
     submedia = (median(z)*tam - max(z))/(tam-1)
     deltaenergia = max(z) - submedia

     txt_x = STRING(FORMAT='("Matrix: ",A)',name_in)
     IF (Sel EQ 'Y') THEN BEGIN
     WSET, DRAW1_Id
         CONTOUR,z,NLEVELS=8,ZRANGE=[0,max(z)],TITLE='Contour-Pattern', $
         XRANGE=[0,mx], YRANGE=[0,my],XTITLE=txt_x
     ENDIF

     dx=gradx(z,mx,my)
     dy=grady(z,mx,my)


; Plotting the total of vectors

     onde = WHERE((dx NE 0) OR (dy NE 0))
     IF (onde(0) NE -1) THEN nvect= N_ELEMENTS(onde) ELSE nvect= 0
     txt_x = STRING(FORMAT='("N=", I6)',nvect)

     IF (Sel EQ 'Y') THEN BEGIN
     WSET, DRAW3_Id
     VELOVECT, dx, dy, DOTS=0,XTITLE=txt_x, TITLE="Gradient Field'
     ENDIF

     factor=(1/SQRT((ABS(MAX(dx)))^2+(ABS(MAX(dy)))^2))/2

; Calculates the alive vectors in the Asymmetric Field

     tparcial =  tempo
     FOR i=0,tam-2 DO BEGIN
    auxx=0
    auxy=0
    norma=0
    anula = 0
    auxx=dx(i+1:tam-1)+dx(i)
    auxy=dy(i+1:tam-1)+dy(i)
    norma=SQRT((auxx)^2+(auxy)^2)
    anula = WHERE(norma LE tol)
    IF anula(0) NE -1 THEN BEGIN
       dx(i)=0
       dy(i)=0
       dx(anula(0)+i+1)=0
       dy(anula(0)+i+1)=0
    END
        segs = SYSTIME(1) - tparcial
        IF (segs gt float(mx/2)/5) THEN BEGIN
         tparcial = SYSTIME(1) - tempo
            str=STRING(FORMAT='(I2.2,":",I2.2,":",I2.2)',tparcial/3600,(tparcial MOD 3600)/60, ((tparcial MOD 3600) MOD 60))
            WIDGET_CONTROL ,TIME, SET_VALUE=str
            tparcial = SYSTIME(1)
            WIDGET_CONTROL ,SLIDER2, SET_VALUE=i*100./tam
        END
     ENDFOR
     onde = WHERE((dx NE 0) OR (dy NE 0))
     IF (onde(0) NE -1) THEN nvetv= N_ELEMENTS(onde) ELSE nvetv= 0

; Plotting the Asymmetric Field

     IF nvetv NE 0 THEN BEGIN

        txt_x = STRING(FORMAT='("L=", I6)',nvetv)
        IF (Sel EQ "Y") THEN BEGIN
       WSET, DRAW2_Id
           VELOVECT, dx, dy,dots=0, XTITLE=txt_x, TITLE='Assimetric Field'
        ENDIF

    naozero=WHERE((dx NE 0) OR (dy NE 0))
    print, "Numero de Nao zero: ",naozero
    vvx=dx(naozero)*factor + naozero MOD my
    vvy=dy(naozero)*factor + naozero/my

        TRIANGULATE, vvx, vvy, tr, CONNECTIVITY=list
        soma=0
        FOR i=0,(N_ELEMENTS(vvx)-1) DO BEGIN
        l=list(list(i):list(i+1)-1)
        FOR j=0,(N_ELEMENTS(l)-1) DO BEGIN
            IF (i LT l(j)) THEN soma=soma+1
        ENDFOR
        ENDFOR
        fa = float(soma-nvetv)/nvetv
        energia = fa * deltaenergia
        txt_x = STRING(FORMAT='("I=", I6, " Fa=", F7.5," E=",F6.3)',soma, fa,energia)

        IF (Sel EQ "Y") THEN BEGIN
        WSET, DRAW4_Id
        PLOT,vvx,vvy,psym=1, XRANGE=[min(vvx),max(vvx)], $
            YRANGE=[min(vvy),max(vvy)],  XGRID=1, YGRID=1, $
            XTITLE = txt_x, TITLE="Triangulation"
        FOR i=0,N_ELEMENTS(tr)/3-1 DO BEGIN
            t = [tr(*,I),tr(0,i)]
            PLOTS,vvx(t),vvy(t)
        ENDFOR
        ENDIF

     ENDIF ELSE BEGIN
        soma=0
        fa=0
    energia=0
     END
     tempo = SYSTIME(1) - tempo
     Resultados
     mx=totlin
     my = totcol
   ENDWHILE
E_Read:
   CLOSE, u
   FREE_LUN, u
   IF NOT ok THEN return, 5 ELSE return, 0
END

;
; Gerencia o menu File
;

PRO FILE_MENU_Event, Event

  COMMON area_draw, DESENHO, DRAW1, DRAW2, DRAW3, DRAW4, EXISTE
  COMMON message_window, MESSAGE

  COMMON files, name_in, name_out

  CASE Event.Value OF

  'File.Load': BEGIN
    name_in = PICKFILE(/MUST_EXIST, /READ)
    str='Arquivo escolhido: ' + name_in + '.'
    WIDGET_CONTROL, MESSAGE, SET_VALUE=str

    END
  'File.Save': BEGIN
    name_out = PICKFILE(/WRITE)
    str='Espere...'
    WIDGET_CONTROL, MESSAGE, SET_VALUE=str
    Salva, name_out
    END
  'File.Quit': BEGIN
    str='Goodbye...'
    WIDGET_CONTROL, MESSAGE, SET_VALUE=str
    IF (EXISTE EQ 'S') THEN WIDGET_CONTROL, DESENHO, /Destroy
    WIDGET_CONTROL , Event.top, /Destroy
    END
  ENDCASE
END

;
; Gerencia os eventos no menu de impressao
;

PRO PRINT_MENU_Event, Event

  COMMON output_data, nvect, nvetv, soma, tempo, nromat, fa, energia
  COMMON para_print, z, mx, my, dx, dy, tr, vvx, vvy
  COMMON files, name_in, name_out
  COMMON message_window, MESSAGE

  CASE Event.Value OF

  'Print.Contour': BEGIN
         filenameps=name_in + '_cont.ps'
         SET_PLOT, 'ps'
         DEVICE, FILENAME=filenameps
         txt_x = STRING(FORMAT='("Matrix: ",A)',name_in)
         CONTOUR,z,NLEVELS=8,ZRANGE=[0,max(z)],TITLE='Contour-Pattern', $
         XRANGE=[0,mx], YRANGE=[0,my], XTITLE=txt_x
         DEVICE, /CLOSE
         SET_PLOT, 'X'
         WIDGET_CONTROL, MESSAGE, SET_VALUE='Saida postscript gerada.'
    END
  'Print.Gradient Field': BEGIN
        gx = gradx(z,mx,my)
        gy = grady(z,mx,my)
    filenameps=name_in + '_gdfield.ps'
        SET_PLOT, 'ps'
        DEVICE, FILENAME=filenameps
        VELOVECT, gx,gy,XTITLE=STRING(FORMAT='("N=",I6)',nvect),TITLE='Gradient Field'
        DEVICE, /CLOSE
        SET_PLOT, 'X'
        WIDGET_CONTROL, MESSAGE, SET_VALUE='Saida postscript gerada.'
    END
  'Print.Assimetric Field': BEGIN
    filenameps=name_in + '_asmfield.ps'
        SET_PLOT, 'ps'
        DEVICE, FILENAME=filenameps
        VELOVECT, dx,dy,XTITLE=STRING(FORMAT='("L=",I6)',nvetv), TITLE= 'Assymetric Field'
        DEVICE, /CLOSE
        SET_PLOT, 'X'
        WIDGET_CONTROL, MESSAGE, SET_VALUE='Saida postscript gerada.'
    END
  'Print.Triangulation': BEGIN
    filenameps=name_in + '_triang.ps'
        SET_PLOT, 'ps'
        DEVICE, FILENAME=filenameps
    PLOT,vvx,vvy,psym=1, XRANGE=[min(vvx),max(vvx)], $
        YRANGE=[min(vvy),max(vvy)],  XGRID=1, YGRID=1, TITLE="Triangulation", $
    XTITLE=STRING(FORMAT='("I=", I6, " Fa=", F7.5, " E=",F6.3)',soma, fa,energia)
        FOR i=0,N_ELEMENTS(tr)/3-1 DO BEGIN
        t = [tr(*,I),tr(0,i)]
            PLOTS,vvx(t),vvy(t)
    ENDFOR
        DEVICE, /CLOSE
        SET_PLOT, 'X'
        WIDGET_CONTROL, MESSAGE, SET_VALUE='Saida postscript gerada.'
    END
  'Print.All': BEGIN
        gx = gradx(z,mx,my)
        gy = grady(z,mx,my)

        !P.MULTI=[0,2,2]
    filenameps=name_in + '_total.ps'
        SET_PLOT, 'ps'
        DEVICE, FILENAME=filenameps

        CONTOUR,z,NLEVELS=8,ZRANGE=[0,max(z)],TITLE='Contour-Pattern', $
        XRANGE=[0,mx], YRANGE=[0,my], XTITLE= STRING(FORMAT='("Matrix: ",A)',name_in)

        VELOVECT, gx,gy,XTITLE=STRING(FORMAT='("N=",I6)',nvect),TITLE='Gradient Field'

        VELOVECT, dx,dy, XTITLE=STRING(FORMAT='("L=",I6)',nvetv), TITLE= 'Assymetric Field'

    PLOT,vvx,vvy,psym=1, XRANGE=[min(vvx),max(vvx)], $
        YRANGE=[min(vvy),max(vvy)],  XGRID=1, YGRID=1, TITLE="Triangulation", $
    XTITLE=STRING(FORMAT='("I=", I6, " Fa=", F7.5, " E=",F6.3)',soma, fa, energia)
        FOR i=0,N_ELEMENTS(tr)/3-1 DO BEGIN
        t = [tr(*,I),tr(0,i)]
            PLOTS,vvx(t),vvy(t)
    ENDFOR

        DEVICE, /CLOSE
        SET_PLOT, 'X'
        WIDGET_CONTROL, MESSAGE, SET_VALUE='Saida postscript gerada.'
    END
  ENDCASE
END

;
;  Gerencia todos os eventos na interface principal
;
PRO CalcFrag_Event, Event

  COMMON output_window, TOTALVECT, ALIVE_VECT, RETAS, FRAG, MAT_NUMBER, TIME, ENERG, SLIDER2
  COMMON area_draw, DESENHO, DRAW1, DRAW2, DRAW3, DRAW4, EXISTE
  COMMON submat_window, SUBMATRIX
  COMMON message_window, MESSAGE

  COMMON input_data, totlin, totcol, tol, Sel, x0, x1, y0, y1,submat
  COMMON output_data, nvect, nvetv, soma, tempo, nromat, fa, energia

  WIDGET_CONTROL,Event.Id,GET_UVALUE=Ev

  CASE Ev OF

  'FileOpt': FILE_MENU_Event, Event
  'Print': PRINT_MENU_Event, Event
  'Lines': BEGIN
    WIDGET_CONTROL, GET_VALUE=totlin, Event.Id
        x1=totlin
      END
  'Columns': BEGIN
    WIDGET_CONTROL, GET_VALUE=totcol, Event.Id
        y1=totcol
      END
  'Tol': BEGIN
      END
  'SubMatriz': BEGIN
      IF Event.Select THEN BEGIN
         submat = 'Y'
         WIDGET_CONTROL, SUBMATRIX, SENSITIVE=1
      ENDIF ELSE BEGIN
         submat = 'N'
         WIDGET_CONTROL, SUBMATRIX, SENSITIVE=0
      END
      END
  'X0': BEGIN
    WIDGET_CONTROL, GET_VALUE=x0, Event.Id
      END
  'Y0': BEGIN
    WIDGET_CONTROL, GET_VALUE=y0, Event.Id
      END
  'X1': BEGIN
    WIDGET_CONTROL, GET_VALUE=x1, Event.Id
      END
  'Y1': BEGIN
    WIDGET_CONTROL, GET_VALUE=y1, Event.Id
      END
  'Saida': BEGIN
      IF Event.Select THEN BEGIN
         Sel = 'Y'
      ENDIF ELSE BEGIN
             PRINT, 'Existe: ', EXISTE
             IF (EXISTE EQ 'S') THEN WIDGET_CONTROL, DESENHO, /Destroy
             Sel = 'N'
             EXISTE = 'N'
      END
      END
  'Run': BEGIN
        Initialise
        result = Calcula_I()
        IF result EQ 0 THEN BEGIN
           WIDGET_CONTROL, MESSAGE, SET_VALUE='Feito!'
           WIDGET_CONTROL, SLIDER2, SET_VALUE=100
        ENDIF ELSE BEGIN
       Trata_erro, ERRO=Result
           WIDGET_CONTROL, SLIDER2, SET_VALUE=0
    END
      END
  'Stop': BEGIN
      Print, 'Event for Stop'
      write_gif,"teste.gif",tvrd()
      END
  ENDCASE
END

;
; Constroi a interface principal do programa
;

PRO CalcFrag, GROUP=Group

  COMMON output_window, TOTALVECT, ALIVE_VECT, RETAS, FRAG, MAT_NUMBER,TIME, ENERG, SLIDER2
  COMMON area_draw, DESENHO, DRAW1, DRAW2, DRAW3, DRAW4, EXISTE
  COMMON submat_window, SUBMATRIX
  COMMON message_window, MESSAGE

  COMMON input_data, totlin, totcol, tol, Sel, x0, x1, y0, y1,submat
  COMMON output_data, nvect, nvetv, soma, tempo, nromat, fa, energia
  COMMON text_sai, texto, ultima_linha

; Initializations:
  mx=0
  my=0
  tol=0
  x0=0
  x1=0
  y0=0
  y1=0
  Sel = 'N'
  submat = 'n'
  EXISTE = 'N'
  ultima_linha = 4
  texto=strarr(100)

  IF N_ELEMENTS(Group) EQ 0 THEN GROUP=0

  junk   = { CW_PDMENU_S, flags:0, name:'' }


  PRINCIPAL = WIDGET_BASE(GROUP_LEADER=Group, $
      COLUMN=1, $
      MAP=1, $
      TITLE='AAF OPERATOR (R R Rosa)', $
      UVALUE='PRINCIPAL')

  BASE1 = WIDGET_BASE(PRINCIPAL, $
      ROW=1, $
      SPACE=5, $
      FRAME=1, $
      MAP=1, $
      TITLE='Top', $
      UVALUE='BASE1')

  MenuDesc360 = [ $
        { CW_PDMENU_S,       3, 'File' }, $ ;        0
        { CW_PDMENU_S,       0, 'Load' }, $ ;        1
        { CW_PDMENU_S,       0, 'Save' }, $ ;        2
        { CW_PDMENU_S,       2, 'Quit' } $  ;      3

  ]


  FILE_MENU = CW_PDMENU( BASE1, MenuDesc360, /RETURN_FULL_NAME, $
      UVALUE='FileOpt', FONT='7x14bold')

  FMenuDesc = [ $
      { CW_PDMENU_S,       3, 'Print' }, $ ;        0
        { CW_PDMENU_S,       0, 'Contour' }, $ ;        1
        { CW_PDMENU_S,       0, 'V =' }, $ ;        2
        { CW_PDMENU_S,       0, 'L =' }, $ ;        3
        { CW_PDMENU_S,       0, 'I = , Fa = ' }, $ ;        4
        { CW_PDMENU_S,       2, 'All' } $  ;      5

  ]


  PRINT_MENU = CW_PDMENU( BASE1, FMenuDesc, /RETURN_FULL_NAME, $
      UVALUE='Print', FONT='7x14bold' )


  MATRIX = WIDGET_BASE(PRINCIPAL, $
      COLUMN=2, $
      SPACE=5, $
      FRAME=1, $
      MAP=1, $
      TITLE='Matrix', $
      UVALUE='MATRIX')

  INPUT_BASE = WIDGET_BASE(MATRIX, $
      COLUMN=1, $
      FRAME=1, $
      MAP=1, $
      TITLE='Dimension', $
      UVALUE='INPUT_BASE')

  FLINES = CW_FIELD( INPUT_BASE,VALUE='', $
      ROW=1,  FONT='7X14bold',$
      INTEGER=1, FIELDFONT='7x14', $
      ALL_EVENTS=1, $
      TITLE='Lines    ', $
      UVALUE='Lines')

   FCOLUMNS = CW_FIELD( INPUT_BASE,VALUE=' ', $
      ROW=1,  FONT='7X14bold', $
      INTEGER=1, FIELDFONT='7x14', $
      ALL_EVENTS=1, $
      TITLE='Columns  ', $
      UVALUE='Columns')

  FTOLERANCE = CW_FIELD( INPUT_BASE, VALUE=0, $
      ROW=1, FONT='7X14bold', $
      FLOAT=1, FIELDFONT='7x14', $
      ALL_EVENTS=1, $
      TITLE='Tolerance', $
      UVALUE='Tol')

 Btns3010 = [ $
    '  ' ]

  BGROUP54 = CW_BGROUP( INPUT_BASE, Btns3010, $
      ROW=1,  FONT='7X14bold',$
      NONEXCLUSIVE = 1,$
      LABEL_LEFT='SubMatrix', /FRAME, $
      UVALUE='SubMatriz')


  SUBMATRIX = WIDGET_BASE(MATRIX, $
      COLUMN=2, $
      FRAME=1, $
      MAP=1, $
      TITLE='DimSubMat', $
      UVALUE='SUBMATRIX')

  INICIAL_LINE = CW_FIELD( SUBMATRIX,VALUE=' ', $
      ROW=1,  FONT='7X14bold',$
      INTEGER=1,  FIELDFONT='7x14',$
      ALL_EVENTS=1, $
      TITLE='Inicial Line  ', $
      UVALUE='X0')


  INICIAL_COL = CW_FIELD( SUBMATRIX,VALUE=' ', $
      ROW=1,  FONT='7X14bold',$
      INTEGER=1,  FIELDFONT='7x14',$
      ALL_EVENTS=1, $
      TITLE='Inicial Column', $
      UVALUE='Y0')

  FINAL_LINE = CW_FIELD( SUBMATRIX,VALUE=' ', $
      ROW=1, FONT='7X14bold', $
      INTEGER=1, FIELDFONT='7x14', $
      ALL_EVENTS=1, $
      TITLE='Final Line  ', $
      UVALUE='X1')


  FINAL_COL = CW_FIELD( SUBMATRIX,VALUE=' ', $
      ROW=1,  FONT='7X14bold',$
      INTEGER=1, FIELDFONT='7x14', $
      ALL_EVENTS=1, $
      TITLE='Final Column', $
      UVALUE='Y1')


  OUTPUT_BASE = WIDGET_BASE(PRINCIPAL, $
      COLUMN=3, $
      FRAME=1, $
      MAP=1, $
      TITLE='Output', $
      UVALUE='OUTPUT_BASE')


  TOTALVECT = CW_FIELD( OUTPUT_BASE,VALUE=' ', $
      ROW=1, FONT='7X14bold', $
      INTEGER=1,  FIELDFONT='7x14',$
      ALL_EVENTS=1, $
      TITLE='V  ', $
      UVALUE='TotalVect')

  ALIVE_VECT = CW_FIELD( OUTPUT_BASE,VALUE=' ', $
      ROW=1, FONT='7X14bold', $
      INTEGER=1,  FIELDFONT='7x14',$
      ALL_EVENTS=1, $
      TITLE='L  ', $
      UVALUE='AliveVect')

  RETAS = CW_FIELD( OUTPUT_BASE,VALUE=' ', $
      ROW=1, FONT='7X14bold', $
      INTEGER=1,  FIELDFONT='7x14',$
      ALL_EVENTS=1, $
      TITLE='I  ', $
      UVALUE='Retas')


  FRAG = CW_FIELD( OUTPUT_BASE,VALUE=' ', $
      ROW=1, FONT='7X14bold', $
      FLOAT=1, FIELDFONT='7x14', $
      ALL_EVENTS=1, $
      TITLE='Fa ', $
      UVALUE='XXX')

  ENERG = CW_FIELD( OUTPUT_BASE,VALUE=' ', $
      ROW=1, FONT='7X14bold', $
      FLOAT=1, FIELDFONT='7x14', $
      ALL_EVENTS=1, $
      TITLE='Efa ', $
      UVALUE='Energia')

  MAT_NUMBER = CW_FIELD( OUTPUT_BASE,VALUE=1, $
      ROW=1,  FONT='7X14bold',$
      INTEGER=1,  FIELDFONT='7x14',$
      ALL_EVENTS=1, $
      TITLE='Matrix', $
      UVALUE='MatNumber')



  COMMANDS_BASE = WIDGET_BASE(PRINCIPAL, $
      ROW=1, $
      SPACE=10, $
      FRAME=1, $
      MAP=1, $
      TITLE='Commands', $
      UVALUE='COMMANDS_BASE')

  BASE53 = WIDGET_BASE(COMMANDS_BASE, $
      ROW=1, $
      MAP=1, $
      UVALUE='BASE53')


  Btns3009 = [ $
    '  ' ]
  BGROUP54 = CW_BGROUP( COMMANDS_BASE, Btns3009, $
      ROW=1,  FONT='7X14bold',$
      NONEXCLUSIVE = 1, $
      LABEL_LEFT='Graphics', /FRAME, $
      UVALUE='Saida')

  TIME = CW_FIELD( COMMANDS_BASE,VALUE=' ', $
      ROW=1,  FONT='7X14bold',$
      STRING=1,  FIELDFONT='7x14',$
      ALL_EVENTS=1, $
      TITLE='Time', $
      UVALUE='Time')

  SLIDER2 = WIDGET_SLIDER( COMMANDS_BASE , $
      FONT='7X14bold',$
      MAXIMUM=100, $
      MINIMUM=0, $
      TITLE='% Processado    ', $
      UVALUE='porcent', $
      VALUE=0)

  RUN_BUT = WIDGET_BUTTON( COMMANDS_BASE, $
      UVALUE='Run',  FONT='7X14bold', $
      VALUE=' Run ')

  STOP_BUT = WIDGET_BUTTON( COMMANDS_BASE, $
      UVALUE='Stop', FONT='7X14bold', $
      VALUE='Stop')

  MESSAGE = WIDGET_TEXT( PRINCIPAL, $
      UVALUE='Message', FONT='times_italic12', $
      XSIZE= 70, /FRAME, $
      VALUE= '    ')


  DESENHO = WIDGET_BASE(GROUP_LEADER=Group, $
      COLUMN=2, $
      MAP=1, $
      TITLE='Calculo da Fragmentacao', $
      UVALUE='DESENHO')

  DRAW1 = WIDGET_DRAW( DESENHO, $
      BUTTON_EVENTS=1, $
      RETAIN=2, $
      UVALUE='DRAW1', $
      XSIZE=350, $
      YSIZE=350)

  DRAW2 = WIDGET_DRAW( DESENHO, $
      BUTTON_EVENTS=1, $
      RETAIN=2, $
      UVALUE='DRAW2', $
      XSIZE=350, $
      YSIZE=350)

  DRAW3 = WIDGET_DRAW( DESENHO, $
      BUTTON_EVENTS=1, $
      RETAIN=2, $
      UVALUE='DRAW3', $
      XSIZE=350, $
      YSIZE=350)

  DRAW4 = WIDGET_DRAW( DESENHO, $
      BUTTON_EVENTS=1, $
      RETAIN=2, $
      UVALUE='DRAW4', $
      XSIZE=350, $
      YSIZE=350)

  WIDGET_CONTROL, PRINCIPAL, /REALIZE
  WIDGET_CONTROL, SUBMATRIX, SENSITIVE=0

  XMANAGER, 'CalcFrag', PRINCIPAL
END
