package xxl.app.renderers;

import xxl.cells.renderers.Renderer;
import xxl.cells.ReferenceContent;
import xxl.cells.IntegerBinary;
import xxl.cells.StringRange;
import xxl.cells.IntegerRange;
import xxl.cells.Range;
import xxl.cells.Binary;

public class OutputRenderer extends Renderer {
	/** @See xxl.cells.renderers.Renderer#renderReferenceContent */
	public String renderReferenceContent(ReferenceContent content) {
		return content.render(_value) + "=" + content.render(_expression);
	}

	/** @See xxl.cells.renderers.Renderer#renderIntegerBinary */
	public String renderIntegerBinary(IntegerBinary content) {
		return renderBinary(content);
	}

	/** @See xxl.cells.renderers.Renderer#renderStringRange */
	public String renderStringRange(StringRange content) {
		return renderRange(content);
	}

	/** @See xxl.cells.renderers.Renderer#renderIntegerRange */
	public String renderIntegerRange(IntegerRange content) {
		return renderRange(content);
	}

	/**
	 * Renders all binaries.
	 *
	 * @param binary to render.
	 * @return rendered binary.
	 */
	private String renderBinary(Binary binary) {
		String result = binary.render(_value) + "=" + binary.render(_expression);
		result += "(" + binary.getFirstOperand().render(_expression);
		return result + "," + binary.getSecondOperand().render(_expression) + ")";
	}

	/**
	 * Renders all ranges.
	 *
	 * @param range to render.
	 * @return rendered range.
	 */
	private String renderRange(Range range) {
		String result = range.render(_value) + "=" + range.render(_expression);
		return result + "(" + range.getInterval().render(this) + ")";
	}

	private ValueRenderer _value = new ValueRenderer();
	private ExpressionRenderer _expression = new ExpressionRenderer();
}
