/*
 * IJA 2022/23: Úloha 2
 * Testovací třída.
 */
package ija.ija2022.homework2;

import java.util.List;
import org.junit.Before;
import org.junit.Test;
import org.junit.Assert;

//--- Importy z implementovaneho reseni ukolu
import ija.ija2022.homework2.game.MazeConfigure;
//---

//--- Importy z baliku dodaneho nastroje
import ija.ija2022.homework2.tool.MazeTester;
import ija.ija2022.homework2.tool.common.CommonField;
import ija.ija2022.homework2.tool.common.CommonMaze;
import ija.ija2022.homework2.tool.common.CommonMazeObject;
//---


/**
 * Testovací třída pro druhý úkol z předmětu IJA 2022/23.
 * @author Radek Kočí
 */
public class Homework2Test {
    
    private CommonMaze maze;
       
    /**
     * Vytvoří bludiště, nad kterým se provádějí testy.
     */
    @Before
    public void setUp() {
        MazeConfigure cfg = new MazeConfigure();
        cfg.startReading(4, 3);
        cfg.processLine("..G");
        cfg.processLine(".X.");
        cfg.processLine(".X.");
        cfg.processLine(".S.");
        cfg.stopReading();       
        maze = cfg.createMaze();         
    }

    /**
     * Test existence objektu, který reprezentuje ducha.
     * 2 body
     */    
    @Test
    public void testGhosts() {
        List<CommonMazeObject> lstGhost = maze.ghosts();
        Assert.assertEquals("Bludiste obsahuje jednoho ducha", 1, lstGhost.size());
        CommonMazeObject obj = lstGhost.remove(0);
        Assert.assertEquals("Bludiste obsahuje jednoho ducha", 1, maze.ghosts().size());
        Assert.assertFalse("Objekt neni pacman", obj.isPacman());
        Assert.assertEquals("Objekt je na spravne pozici",
                maze.getField(1, 3),
                obj.getField());
    }
 
    /**
     * Test správného pohybu ducha po bludišti.
     * 2 body
     */    
    @Test
    public void testGhostMoving() {
        // Ghost na pozici 1,3
        CommonMazeObject obj = maze.ghosts().get(0);
        Assert.assertTrue("Presun na policko se podari.", obj.move(CommonField.Direction.D));
        Assert.assertTrue("Presun na policko se podari.", obj.move(CommonField.Direction.D));
        Assert.assertTrue("Presun na policko se podari.", obj.move(CommonField.Direction.D));
        Assert.assertFalse("Presun na policko se nepodari.", obj.move(CommonField.Direction.R));       
    }    
   
    /**
     * Test správného chování při setkání ducha s pacmanem (sníží se počet životů pacmana).
     * 3 body
     */
    @Test
    public void testGhostMeetsPacman() {
        // Ghost na pozici 1,3
        CommonMazeObject ghost = maze.ghosts().get(0);

        // Pacman na pozici 4,2
        Assert.assertFalse("Policko [4,2] neni prazdne", maze.getField(4, 2).isEmpty());
        CommonMazeObject pacman = maze.getField(4, 2).get();
        Assert.assertTrue("Objekt je pacman", pacman.isPacman());
        Assert.assertEquals("Pocet zivotu pacmana", 3, pacman.getLives());
        
        Assert.assertTrue("Presun na policko se podari.", ghost.move(CommonField.Direction.D));
        Assert.assertEquals("Pocet zivotu pacmana", 3, pacman.getLives());
        Assert.assertTrue("Presun na policko se podari.", ghost.move(CommonField.Direction.D));
        Assert.assertEquals("Pocet zivotu pacmana", 3, pacman.getLives());
        Assert.assertTrue("Presun na policko se podari.", ghost.move(CommonField.Direction.D));
        Assert.assertEquals("Pocet zivotu pacmana", 3, pacman.getLives());
        Assert.assertTrue("Presun na policko se podari.", ghost.move(CommonField.Direction.L));       
        Assert.assertEquals("Pocet zivotu pacmana", 2, pacman.getLives());
    }    

    /**
     * Testování notifikací při přesunu objektu (ducha).
     * 5 bodů
     */
    @Test
    public void testNotificationGhostMoving() {
        MazeTester tester = new MazeTester(maze);        
       
        // Ghost na pozici 1,3
        CommonMazeObject obj = maze.ghosts().get(0);

        /* Testy, kdy se presun podari.
         * Dve prezentace policka (view) budou notifikovana o zmene (odebrani objektu a vlozeni objektu).
         * Kazde takove view bude notifikovano prave jednou.
         * Ostatni notifikovana nebudou.
         */
        testNotificationGhostMoving(tester, true, obj, CommonField.Direction.L);
        testNotificationGhostMoving(tester, true, obj, CommonField.Direction.L);
        testNotificationGhostMoving(tester, true, obj, CommonField.Direction.D);
        
        /* Testy, kdy se presun nepodari (pokus vstoupit do zdi).
         * Nikdo nebude notifikovan.
         */
        testNotificationGhostMoving(tester, false, obj, CommonField.Direction.R);
    }

    /**
     * Pomocná metoda pro testování notifikací při přesunu objektu.
     * @param tester Tester nad bludištěm, který provádí vyhodnocení notifikací.
     * @param success Zda se má přesun podařit nebo ne
     * @param obj Přesouvaný objekt
     * @param dir Směr přesunu
     */
    private void testNotificationGhostMoving(MazeTester tester, boolean success, CommonMazeObject obj, CommonField.Direction dir) {
        StringBuilder msg;
        boolean res;

        // Policko, na kterem byl objekt pred zmenou
        CommonField previous = obj.getField();        
        // Policko, na kterem ma byt objekt po zmene
        CommonField current = previous.nextField(dir);

        // Zadna notifikace zatim neexistuje
        res = tester.checkEmptyNotification();
        Assert.assertTrue("Zadna notifikace.", res);
        
        // Pokud se ma presun podarit
        if (success) {
            Assert.assertTrue("Presun na policko se podari.", obj.move(dir));
            msg = new StringBuilder();
            // Overeni spravnych notifikaci
            res = tester.checkNotification(msg, obj, current, previous);
            Assert.assertTrue("Test notifikace: " + msg, res);
        } 
        // Pokud se nema presun podarit
        else {
            Assert.assertFalse("Presun na policko se nepodari.", obj.move(dir));            
            // Zadne notifikace nebyly zaslany
            res = tester.checkEmptyNotification();
            Assert.assertTrue("Zadna notifikace.", res);
        }
    }
    
}
