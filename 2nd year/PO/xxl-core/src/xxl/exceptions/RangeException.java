package xxl.exceptions;

/**
 * This exception is thrown whenever a range is deformed.
 */
public class RangeException extends Exception {
	public RangeException() { super(); }

	public RangeException(String range) { super(range); }
}
