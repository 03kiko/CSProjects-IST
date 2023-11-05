package xxl.app.renderers;

import xxl.cells.renderers.Renderer;
import xxl.cells.ReferenceContent;
import xxl.cells.IntegerBinary;
import xxl.cells.StringRange;
import xxl.cells.IntegerRange;

public class ExpressionRenderer extends Renderer {
	/** @See Renderer#renderReferenceContent */
	public String renderReferenceContent(ReferenceContent content) {
		return content.getPosition().render(this);
	}

	/** @See Renderer#renderIntegerBinary */
	public String renderIntegerBinary(IntegerBinary content) {
		return content.getOperation().render(this);
	}

	/** @See Renderer#renderStringRange */
	public String renderStringRange(StringRange content) {
		return content.getName();
	}

	/** @See Renderer#renderIntegerRange */
	public String renderIntegerRange(IntegerRange content) {
		return content.getName();
	}
}
