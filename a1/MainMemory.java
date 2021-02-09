package arch.sm213.machine.student;

import machine.AbstractMainMemory;


/**
 * Main Memory of Simple CPU.
 *
 * Provides an abstraction of main memory (DRAM).
 */

public class MainMemory extends AbstractMainMemory {
  private byte [] mem;
  
  /**
   * Allocate memory.
   * @param byteCapacity size of memory in bytes.
   */
  public MainMemory (int byteCapacity) {
    mem = new byte [byteCapacity];
  }
  
  /**
   * Determine whether an address is aligned to specified length.
   * @param address memory address.
   * @param length byte length.
   * @return true iff address is aligned to length.
   */
  @Override protected boolean isAccessAligned (int address, int length) {

    return address % length ==0;
  }
  
  /**
   * Convert an sequence of four bytes into a Big Endian integer.
   * @param byteAtAddrPlus0 value of byte with lowest memory address (base address).
   * @param byteAtAddrPlus1 value of byte at base address plus 1.
   * @param byteAtAddrPlus2 value of byte at base address plus 2.
   * @param byteAtAddrPlus3 value of byte at base address plus 3 (highest memory address).
   * @return Big Endian integer formed by these four bytes.
   */
  @Override public int bytesToInteger (byte byteAtAddrPlus0, byte byteAtAddrPlus1, byte byteAtAddrPlus2, byte byteAtAddrPlus3) {
    String val0 = String.format("%02x",(int)byteAtAddrPlus0 & 0xff);
    String val1 = String.format("%02x",(int)byteAtAddrPlus1 & 0xff);
    String val2 = String.format("%02x",(int)byteAtAddrPlus2 & 0xff);
    String val3 = String.format("%02x",(int)byteAtAddrPlus3 & 0xff);
    String result = val0 + val1 + val2 + val3;


    return (int) Long.parseLong(result,16);
  }
  
  /**
   * Convert a Big Endian integer into an array of 4 bytes organized by memory address.
   * @param  i an Big Endian integer.
   * @return an array of byte where [0] is value of low-address byte of the number etc.
   */
  @Override public byte[] integerToBytes (int i) {
    StringBuilder hex = new StringBuilder(8);
    hex.append(Integer.toHexString(i));
    byte[] memo = new byte[4];
    while (hex.length()<=8) {
      hex.append("0");
    }
    String str = hex.toString();
    for (i = 0; i < 8; i += 2) {
      String digit = "" + str.charAt(i) + str.charAt(i + 1);
      memo[i/2] = Integer.valueOf(digit,16).byteValue();
    }


    return memo;
  }
  
  /**
   * Fetch a sequence of bytes from memory.
   * @param address address of the first byte to fetch.
   * @param length  number of bytes to fetch.
   * @throws InvalidAddressException  if any address in the range address to address+length-1 is invalid.
   * @return an array of byte where [0] is memory value at address, [1] is memory value at address+1 etc.
   */
  @Override protected byte[] get (int address, int length) throws InvalidAddressException {
    if (address + length - 1 >= mem.length) {
      throw new InvalidAddressException();
    }
    byte[] result = new byte[length];
    for (int i = 0; i < length; i++) {
      result[i] = mem[address + i];

    }
    return result;
  }
  
  /**
   * Store a sequence of bytes into memory.
   * @param  address                  address of the first byte in memory to recieve the specified value.
   * @param  value                    an array of byte values to store in memory at the specified address.
   * @throws InvalidAddressException  if any address in the range address to address+value.length-1 is invalid.
   */
  @Override protected void set (int address, byte[] value) throws InvalidAddressException {
    if (address + value.length - 1 >= mem.length) {
      throw new InvalidAddressException();
    }
    for (int i = 0; i < value.length; i++) {
      mem[address + i] = value[i];
    }
  }
  
  /**
   * Determine the size of memory.
   * @return the number of bytes allocated to this memory.
   */
  @Override public int length () {
    return mem.length;
  }
}
