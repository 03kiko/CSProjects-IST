package xxl.exceptions;

/**
 * This exception is thrown when an exception exceeds
 * the spreadsheet's bounds.
 */
public class RangeOutOfBoundsException extends RangeException {
	public RangeOutOfBoundsException() {
		this("");
	}

	/**
	 * @param range the illegal range
	 */
	public RangeOutOfBoundsException(String range) {
		super(range);
	}
}
