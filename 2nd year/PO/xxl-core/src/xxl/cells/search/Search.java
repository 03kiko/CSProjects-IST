package xxl.cells.search;

import xxl.cells.*;
import xxl.parser.Parser;

/**
 * Abstract class used to define search criteria for different content types.
 */
public abstract class Search {
	/**
     	 * Checks if the search is applicable to the integer content.
     	 *
     	 * @param content The integer content to be checked.
     	 * @return True if the search criteria match; false otherwise.
     	 */
	abstract public boolean okIntegerContent(IntegerContent content);

	/**
	 * Checks if the search is applicable to the string content.
	 *
	 * @param content The string content to be checked.
	 * @return True if the search criteria match; false otherwise. 
	 */
	abstract public boolean okStringContent(StringContent content);
	
	/**
	 * Checks if the search is applicable to the reference Content.
	 *
	 * @param content The reference content to be checked.
	 * @return True if the search criteria match; false otherwise.
	 */
	abstract public boolean okReferenceContent(ReferenceContent content);
	
	/**
	 * Checks if the search is applicable to the integer binary.
	 *
	 * @param content The integer binary to be checked.
	 * @return True if the search criteria match; false otherwise.
	 */
	abstract public boolean okIntegerBinary(IntegerBinary content);
	
	/**
	 * Checks if the search is applicable to the integer range.
	 *
	 * @param content The integer ranged to be checked.
	 * @return True if the search criteria match; false otherwise.
	 */
	abstract public boolean okIntegerRange(IntegerRange content);
	
	/**
	 * Checks if the search is applicable to the string range.
	 *
	 * @param content The string range to be checked.
	 * @return True if the search criteria match; false otherwise.
	 */
	abstract public boolean okStringRange(StringRange content);

	protected Parser _parser = new Parser();
}
