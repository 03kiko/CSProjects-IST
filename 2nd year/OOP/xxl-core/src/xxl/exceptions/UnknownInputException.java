package xxl.exceptions;

/**
 * This exception is thrown whenever a given input
 * cannot be materialized into a Content.
 */
public class UnknownInputException extends Exception {
	/**
	 * @param input the unknown input
	 */
	public UnknownInputException(String input) { super(input); }
}
