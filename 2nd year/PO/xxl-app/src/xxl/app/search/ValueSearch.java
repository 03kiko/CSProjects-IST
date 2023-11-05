package xxl.app.search;

import xxl.cells.*;
import xxl.cells.search.Search;
import xxl.exceptions.UnknownInputException;

/**
 * This class represents a search strategy based on specific values
 *
 */
public class ValueSearch extends Search {
	/** @param value The value to be searched */
	public ValueSearch(String value) throws UnknownInputException {
			_value = _parser.readLiteral(value);
	}

	/** @See xxl.cells.search.Search#okIntegerContent */
	@Override
	public boolean okIntegerContent(IntegerContent content) {
		if(_value.getInteger() != null)
			return content.getInteger().equals(_value.getInteger());
		return false;
	}

	/** @See xxl.cells.search.Search#okStringContent */
	@Override
	public boolean okStringContent(StringContent content) {
		if(_value.getString() != null)
			return content.getString().equals(_value.getString());
		return false;
	}

	/** @See xxl.cells.search.Search#okReferenceContent */
	@Override
	public boolean okReferenceContent(ReferenceContent content) {
		if(_value.getInteger() != null)
			return _value.getInteger().equals(content.getInteger());
		return _value.getString().equals(content.getString()); 
	}

	/** @See xxl.cells.search.Search#okIntegerBinary */
	@Override
	public boolean okIntegerBinary(IntegerBinary content) {
		if(_value.getInteger() != null)
			return _value.getInteger().equals(content.getInteger());
		return false;
	}

	/** @See xxl.cells.search.Search#okIntegerRange */
	@Override
	public boolean okIntegerRange(IntegerRange content) {
		if(_value.getInteger() != null)
			return _value.getInteger().equals(content.getInteger());
		return false;
	}

	/** @See xxl.cells.search.Search#okStringRange */
	@Override
	public boolean okStringRange(StringRange content) {
		if(_value.getString() != null)
			return _value.getString().equals(content.getString());
		return false;
	}

	private Content _value;
}
