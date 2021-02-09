package arch.sm213.machine.student;


import machine.AbstractMainMemory;
import org.junit.Test;

import static org.junit.jupiter.api.Assertions.*;

public class MainMemoryTest {
    private byte d = Integer.valueOf("01", 16).byteValue();
    private byte c = Integer.valueOf("00", 16).byteValue();
    private byte b = Integer.valueOf("ff", 16).byteValue();

    //testing integer conversions with bit-eight set to 1
    @Test
    public void toByteTest() {
        MainMemory a = new MainMemory(4);
        int r = a.bytesToInteger(b, b, b, b);
        assertEquals(-1, r);
    }

    //testing more complex integer conversion with bit-eight 0
    @Test
    public void toByteTest1() {
        MainMemory a = new MainMemory(4);
        int r = a.bytesToInteger(c, c, c, d);
        assertEquals(1, r);
        assertEquals(0,a.bytesToInteger(c,c,c,c));
        assertEquals(255,a.bytesToInteger(c,c,c,b));

    }

    //testing various alignment situations
    @Test
    public void alignedTest() {
        MainMemory mem = new MainMemory(4);
        assertTrue(mem.isAccessAligned(0, 4));
        assertFalse(mem.isAccessAligned(3, 5));
        assertTrue(mem.isAccessAligned(6, 3));
        assertFalse(mem.isAccessAligned(4, 8));
    }

    //testing converting from int to byte with 0 and -1
    @Test
    public void intToByteTest() {
        MainMemory mem = new MainMemory(4);
        byte[] result = mem.integerToBytes(0);
        for (int i = 0; i < 4; i++) {
            assertEquals(result[i], 0);
        }
        result = mem.integerToBytes(-1);
        for (int i = 0; i < 4; i++) assertEquals(result[i], (byte) 0xff);
    }

    //testing setting memory to one value and the retrieved bytes has the same value
    @Test
    public void compTest() {
        MainMemory m = new MainMemory(64);
        byte[] bytes = {0,0,0,0};
        byte[] bytes1 = {1};

        try {
            m.set(0, bytes);
            byte[] result = m.get(0,4);
            for (int i = 0; i < 4; i++) {
                assertEquals(result[i],bytes[i]);
            }
            m.set(63,bytes1);
            result = m.get(63,1);
            assertEquals(result[0],bytes1[0]);
        }
        catch(Exception e) {
            fail();

        }
    }

    //testing failed set, out of range
    @Test
    public void getSetFail() {
        MainMemory mem = new MainMemory(8);
        try {
            byte[] failz = {1,2,3,4};
            mem.set(8,failz);
            fail();
        } catch (Exception e) {
        }
    }
}
