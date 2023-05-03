import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.awt.image.WritableRaster;
import java.io.File;
import java.io.IOException;

//доделать:
//задание 4б
//инд.задание 4е
//попраавить гистограммы в 15 задание (их должно быть 24 и доьавит отрицательное крыло)

public class Main {
    public static void main(String[] args) throws IOException {

        File file = new File ("imgs/kodim02.bmp"); // открыли файл
        BufferedImage image = ImageIO.read(file); // считали картинку, можем работать с пикселями

        EditorImage editor = new EditorImage(image);

        System.out.println("Cмещение в байтах от начала структуры до массива пикселей(bfOffBits) равно: 54");

        // задание 3
        editor.getResEx_3();

        // задание 4а
        System.out.println("Задание 4: ");
        System.out.println("Оценка коэф. корреляции между B и G: " + editor.getResEx_4a("blue","green"));
        System.out.println("Оценка коэф. корреляции между R и G: " + editor.getResEx_4a("red","green"));
        System.out.println("Оценка коэф. корреляции между R и B: " + editor.getResEx_4a("red","blue") + "\n");

        // задание 5
        System.out.println("Задание 5: ");
        editor.getResEx_5();

        // задание 6
        editor.getResEx_6();

        // задание 7
        System.out.println("Задание 7: ");
        editor.getResEx_7();

        // задание 8,9,10
        editor.getResEx_8_9_10(2);

        //задание 11
        editor.getResEx_8_9_10(4);

        //задание 12
       // editor.getResEx_12();

        //задание 13
        editor.getResEx_13();

        // задание 14,16
        editor.getResEx_14_16();

        //задание 15
        editor.getResEx_15();
    }
}
