package xxl.cells.renderers;

import xxl.cells.*;

public abstract class Renderer {
	/**
	 * Combines two Renderable objects into a single render
	 * in the following manner:
	 * left|right
	 *
	 * @return the result of the merge.
	 */
	public String combine(Renderable left, Renderable right) {
		String leftRender = (left != null) ? left.render(this) : "";
		String rightRender = (right != null) ? right.render(this) : "";
		return leftRender + "|" + rightRender;
	}

	/**
	 * Renders a string literal.
	 *
	 * @param content to render.
	 * @return rendered literal.
	 */
	public String renderStringContent(StringContent content) {
		return "'" + content.getString();
	}

	/**
	 * Renders an integer literal.
	 *
	 * @param content to render.
	 * @return rendered literal.
	 */
	public String renderIntegerContent(IntegerContent content) {
		return content.getInteger().toString();
	}

	/**
	 * Renders a reference.
	 *
	 * @param content to render.
	 * @return rendered reference.
	 */
	abstract public String renderReferenceContent(ReferenceContent content);

	/**
	 * Renders an IntegerBinary.
	 *
	 * @param content to render.
	 * @return rendered binary.
	 */
	abstract public String renderIntegerBinary(IntegerBinary content);

	/**
	 * Renders a StringRange.
	 *
	 * @param content to render.
	 * @return rendered range.
	 */
	abstract public String renderStringRange(StringRange content);

	/**
	 * Renders an IntegerRange.
	 *
	 * @param content to render.
	 * @return rendered range.
	 */
	abstract public String renderIntegerRange(IntegerRange content);

	/**
	 * Renders a binary operation.
	 *
	 * @param content to render.
	 * @return rendered operation.
	 */
	public String renderBinaryOperation(BinaryOperation operation) {
		return operation.getName();
	}

	/**
	 * Renders a position.
	 *
	 * @param position to render.
	 * @return rendered position.
	 */
	public String renderPosition(Position position) {
		return position.toString();
	}

	/**
	 * Renders an interval.
	 *
	 * @param interval to render.
	 * @return rendered interval.
	 */
	public String renderInterval(Interval interval) {
		return interval.toString();
	}
}
