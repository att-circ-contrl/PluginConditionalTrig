# TTL Conditional Triggering Plugin - To-Do List

* Make a companion widget that reads integer-valued codes and converts them
to boolean values to feed to this widget. This could be set up as a detector
or set up as a demultiplexer (each output recognizes a number and only the
most recently recognized is active within a group of outputs).

* Maybe make inputs disable themselves if input channel and bit aren't valid.

* Logic bug: Not all outputs are propagating on initialization. This happens
when the steady-state output of a condition processor is high but the merge
stage behind it gets its input initialized to low. I'm not sure if this is
due to ignoring active-high/active-low output state or from situations
where the condition is active-high but looking for low input or if both of
these occur. TLDR, at least one input update should occur before outputs
are considered valid.

_(This is the end of the file.)_
