package xxl.app.search;

import xxl.cells.*;
import xxl.cells.search.Search;

/**
 * This class represents a search strategy based on specific functions
 *
 */
public class FunctionSearch extends Search {
	/** @param functionName The function name to be searched */
	public FunctionSearch(String functionName) {
		_functionName = functionName;
	}

	/** @See xxl.cells.search.Search#okIntegerContent */
	@Override
	public boolean okIntegerContent(IntegerContent content) {
		return false;
	}

	/** @See xxl.cells.search.Search#okStringContent */
	@Override
	public boolean okStringContent(StringContent content) {
		return false;
	}

	/** @See xxl.cells.search.Search#okReferenceContent */
	@Override
	public boolean okReferenceContent(ReferenceContent content) {
		return false;
	}

	/** @See xxl.cells.search.Search#okIntegerBinary */
	@Override
	public boolean okIntegerBinary(IntegerBinary content) {
		return content.getOperation().getName().contains(_functionName);
	}

	/** @See xxl.cells.search.Search#okIntegerRange */
	@Override
	public boolean okIntegerRange(IntegerRange content) {
			return content.getName().contains(_functionName);
	}

	/** @See xxl.cells.search.Search#okStringRange */
	@Override
	public boolean okStringRange(StringRange content) {
			return content.getName().contains(_functionName);
	}

	private String _functionName;
}
