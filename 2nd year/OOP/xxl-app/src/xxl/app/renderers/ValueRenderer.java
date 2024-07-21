package xxl.app.renderers;

import xxl.cells.renderers.Renderer;
import xxl.cells.ReferenceContent;
import xxl.cells.IntegerBinary;
import xxl.cells.StringRange;
import xxl.cells.IntegerRange;

public class ValueRenderer extends Renderer {
	/** @See Renderer#renderReferenceContent */
	@Override
	public String renderReferenceContent(ReferenceContent content) {
		String result = content.getReferenced().render(this);
		if (result.equals(""))
			return "#VALUE";
		return result;
	}

	/** @See Renderer#renderIntegerBinary */
	@Override
	public String renderIntegerBinary(IntegerBinary content) {
		Integer result = content.getInteger();
		if (result == null)
			return "#VALUE";
		return result.toString();
	}

	/** @See Renderer#renderStringRange */
	@Override
	public String renderStringRange(StringRange content) {
		String result = content.getString();
		if (result == null)
			return "#VALUE";
		return "'" + result;
	}

	/** @See Renderer#renderIntegerRange */
	@Override
	public String renderIntegerRange(IntegerRange content) {
		Integer result = content.getInteger();
		if (result == null)
			return "#VALUE";
		return result.toString();
	}
}
