package xxl.exceptions;

/**
 * This exception is thrown whenever a range cannot be read correcly.
 */
public class UnknownRangeException extends RangeException {
	/**
	 * @param interval bad input
	 */
	public UnknownRangeException(String interval) {
		super(interval);
	}
}
