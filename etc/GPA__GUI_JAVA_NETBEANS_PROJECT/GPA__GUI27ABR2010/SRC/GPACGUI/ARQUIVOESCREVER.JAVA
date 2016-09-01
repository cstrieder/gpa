/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package gpacgui;

import java.io.BufferedOutputStream;
import java.io.DataOutputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Administrador
 */

public class ArquivoEscrever {

    public static FileOutputStream file = null;
    public static DataOutputStream dados = null;

    public static void createW(String arquivo) {
        try {
            file = new FileOutputStream(arquivo, true);
            dados = new DataOutputStream(new BufferedOutputStream(file));
        } catch (FileNotFoundException ex) {
            Logger.getLogger(ArquivoEscrever.class.getName()).log(Level.SEVERE, null, ex);
        }

    }

    public static void write(String stringRW) {
        try {
            dados.writeChars(stringRW);
//            dados.writeBytes(stringRW);
        } catch (IOException ex) {
            Logger.getLogger(ArquivoEscrever.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public static void closeW() {
        try {
            dados.flush();
            dados.close();
            file.flush();
            file.close();
        } catch (IOException ex) {
            Logger.getLogger(ArquivoEscrever.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
}
