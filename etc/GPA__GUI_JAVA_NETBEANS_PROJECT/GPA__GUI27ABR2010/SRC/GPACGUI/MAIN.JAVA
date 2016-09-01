/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package gpacgui;

import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

/**
 *
 * @author cristiano
 */
public class Main {

    public static Window tela;
    static String fpath;
    public static ArquivoEscrever myfileW = new ArquivoEscrever();
    public static String filePath = "";
    public static String home  = "";
    static String outdir;
    static String destiny;
    static String currentdir;

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        try {
            // TODO code application logic here
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (ClassNotFoundException ex) {
            Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
        } catch (UnsupportedLookAndFeelException ex) {
            Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
        }
        tela = new Window();
        tela.setLocationRelativeTo(null);
        tela.setTitle("gpa++GUI");
        tela.setVisible(true);

//        Main.tela.ReadConfFromFile();
    }
}
