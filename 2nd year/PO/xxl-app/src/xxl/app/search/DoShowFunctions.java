package xxl.app.search;

import pt.tecnico.uilib.menus.Command;
import xxl.Spreadsheet;
import xxl.app.renderers.OutputRenderer;
import xxl.app.renderers.ExpressionRenderer;
import xxl.cells.search.PositionCellPair;
import java.util.List;
import java.util.Collections;
import java.util.Comparator;

/**
 * Command for searching function names.
 */
class DoShowFunctions extends Command<Spreadsheet> {

	DoShowFunctions(Spreadsheet receiver) {
		super(Label.SEARCH_FUNCTIONS, receiver);
		addStringField("function", Prompt.searchFunction());
	}

	@Override
	protected final void execute() {
		OutputRenderer renderer = new OutputRenderer();
		List<PositionCellPair> ok = _receiver.search(new FunctionSearch(stringField("function")));

		Collections.sort(ok, new Comparator<PositionCellPair>() {
			ExpressionRenderer renderer = new ExpressionRenderer();

			@Override
			public int compare(PositionCellPair p1, PositionCellPair p2) {
				return String.CASE_INSENSITIVE_ORDER
					.compare(p1.getCell().render(renderer),
						p2.getCell().render(renderer));
			}
		});

		for(PositionCellPair pair: ok)
			_display.addLine(renderer.combine(pair.getPosition(), pair.getCell()));
		_display.display();
	}

}
