package xxl.exceptions;

/**
 * This exception is thrown whenever a range isn't a straight line,
 * be it vertically or horizontally.
 */
public class NonLinearRangeException extends RangeException {
	public NonLinearRangeException() {
		this("");
	}

	/**
	 * @param range the illegal range
	 */
	public NonLinearRangeException(String range) {
		super(range);
	}
}
